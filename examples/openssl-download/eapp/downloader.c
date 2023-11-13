#include "app/eapp_utils.h"
#include "app/export.h"
#include "app/syscall.h"
#include "fcntl.h"
#include <ucontext.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

/*
Notes:
- Do { sudo sysctl -w net.ipv4.ping_group_range='0 2147483647' } on HOST machine
*/

#define BUF_SIZE 1024
#define RESPONSE_BUF_SIZE 16384

// #define HOST_NAME "www.google.com"
// #define FILE_TO_DOWNLOAD "images/branding/googlelogo/2x/googlelogo_light_color_272x92dp.png"
#define HOST_NAME "homes.cs.washington.edu"
#define FILE_TO_DOWNLOAD "~chungmcl/files/8.png"

#define HEADER_FILE "/tmp/header.txt"
#define CONTENT_FILE "/tmp/content.png"

extern unsigned int certificate_bytes_len;
extern unsigned char certificate_bytes[];

#define print_errors_and_clean_openssl(ctx, connection_bio, certificate_bio, x509) __print_errors_and_clean_openssl(__LINE__, ctx, connection_bio, certificate_bio, x509)
void __print_errors_and_clean_openssl(int line, SSL_CTX* ctx, BIO* connection_bio, BIO* certificate_bio, X509* x509);

int main()
{ 
  SSL_library_init(); //SSL_load_error_strings();

  const SSL_METHOD* method = TLS_client_method();
  if (method == NULL) return -1;

  SSL_CTX* ctx = SSL_CTX_new(method);
  if (ctx == NULL) return -1;

  BIO* certificate_bio = NULL;
  X509* cert = NULL;
  certificate_bio = BIO_new_mem_buf((void*)certificate_bytes, certificate_bytes_len);

  // Loads PEM certificate(s) from .crt or .pem file specified in CMakeLists.txt.
  // There may be more than one PEM certificate in the file. Reads them all in the while loop.
  while ((cert = PEM_read_bio_X509(certificate_bio, NULL, 0, NULL)) != NULL) {
    X509_STORE_add_cert(SSL_CTX_get_cert_store(ctx), cert);
  }
  // Loads all certificates in /etc/ssl/certs in UNTRUSTED Linux host.
  // if (!SSL_CTX_load_verify_locations(
  //   ctx, 
  //   "/etc/ssl/certs/ca-certificates.crt", 
  //   "/etc/ssl/certs/"
  //   )
  // ) {
  //   print_errors_and_clean_openssl(ctx, NULL, certificate_bio, cert);
  // }

  BIO* connection_bio = BIO_new_ssl_connect(ctx);
  if (connection_bio == NULL) {
    // ctx is automatically freed when BIO_new_ssl_connect fails
    print_errors_and_clean_openssl(NULL, NULL, certificate_bio, cert);
    return -1;
  }

  SSL* ssl = NULL; // automatically freed when ctx is freed
  BIO_get_ssl(connection_bio, &ssl);
  SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

  char name[BUF_SIZE];
  sprintf(name, "%s:%s", HOST_NAME, "https"); 
  BIO_set_conn_hostname(connection_bio, name); 
  
  if (BIO_do_connect(connection_bio) <= 0) {
    printf("!!! BIO_do_connect failed! error: %d\n", errno);
    print_errors_and_clean_openssl(ctx, connection_bio, certificate_bio, cert);
    return -1;
  }
  
  long verify_flag = SSL_get_verify_result(ssl); 
  if (verify_flag != X509_V_OK) {
    // https://www.openssl.org/docs/man1.0.2/man1/verify.html
    printf("!!! SSL_get_verify_result not successful: (%i)\n", (int)verify_flag);
    print_errors_and_clean_openssl(ctx, connection_bio, certificate_bio, cert);
    return -1;
  }
  printf("!!! SSL_get_verify_result succeeded!\n");

  char request[BUF_SIZE];
  sprintf(request, 
          "GET /%s HTTP/1.1\r\n"
          "Host: %s\r\n"
          "Connection: Close\r\n"
          "\r\n",
          FILE_TO_DOWNLOAD,
          HOST_NAME); 
  BIO_puts(connection_bio, request);

  char* response = malloc(RESPONSE_BUF_SIZE);
  char* header = malloc(BUF_SIZE); char* header_cur = header;
  char* content; char* content_cur;
  bool header_finished = false;

  const char* end_of_header_token = "\r\n\r\n";
  const int end_of_header_token_len = 4;
  while (true) { 
    memset(response, '\0', RESPONSE_BUF_SIZE);
    int n = BIO_read(connection_bio, response, RESPONSE_BUF_SIZE);
    printf("bytes read from BIO_read: %d\n", n);
    if (n == 0) 
      break;
    else if (n < 0) {
      print_errors_and_clean_openssl(ctx, connection_bio, certificate_bio, cert);
      printf("!!! BIO_read failed\n");
      return -1;
    }

    if (!header_finished) {
      char* end_of_header_token_ptr = strstr(response, end_of_header_token);
      if (end_of_header_token_ptr == NULL) {
        memcpy(header_cur, response, n);
        header_cur += n;
      } else {
        memcpy(header_cur, response, end_of_header_token_ptr + end_of_header_token_len - response);
        const char* content_length_token = "Content-Length: ";
        const int content_length_token_len = 16;
        char* content_length_str = strstr(header, content_length_token) + content_length_token_len;
        char* content_length_str_end = strchr(content_length_str, '\r');
        *content_length_str_end = '\0';
        long content_length = atol(content_length_str);
        *content_length_str_end = '\r';
        content = malloc(content_length);
        content_cur = content;

        memcpy(content_cur, end_of_header_token_ptr + end_of_header_token_len, (response + n) - (end_of_header_token_ptr + end_of_header_token_len));
        content_cur += (response + n) - (end_of_header_token_ptr + end_of_header_token_len);
        header_cur[end_of_header_token_ptr + end_of_header_token_len - response] = '\0';
        header_finished = true;
      }
    } else {
      memcpy(content_cur, response, n);
      content_cur += n;
    }
  }
  printf("!!! Sucessfully downloaded to memory\n");
  free(response);
  print_errors_and_clean_openssl(ctx, connection_bio, certificate_bio, cert);

  FILE* header_file = fopen(HEADER_FILE, "w");
  fputs(header, header_file);
  fclose(header_file);

  FILE* data_file = fopen(CONTENT_FILE, "w");
  for (uint64_t i = 0; i < (content_cur - content); i += 1) {
    fputc(content[i], data_file);
  }
  fclose(data_file);
  printf("!!! Finished writing header and content to %s and %s\n", HEADER_FILE, CONTENT_FILE);

  char device_name[] = "serial@10001000";
  if (claim_mmio(device_name, strlen(device_name)) >= 0) {
    // char uart8250_name[] = "serial";
    int uart8250_fd = openat(-2, device_name, 0, 0);
    if (uart8250_fd >= 0) {
      char* cur_pos = content;
      printf("!!! Beginning UART write to serial.out\n");
      while (cur_pos < content_cur) {
        cur_pos += write(uart8250_fd, cur_pos, content_cur - cur_pos);
      }
    } else {
      printf("!!! Received negative FD\n");
      free(header); free(content); return -1;
    }
  } else {
    printf("!!! Failed to claim uart\n");
    free(header); free(content); return -1;
  }

  free(header);
  free(content);
  printf("!!! downloader finished.\n");
  return 0;
}

void __print_errors_and_clean_openssl(int line, SSL_CTX* ctx, BIO* connection_bio, BIO* certificate_bio, X509* x509) {
  printf("!!! Cleaning up on line %d\n", line);
  ERR_print_errors_fp(stdout);
  if (ctx != NULL) SSL_CTX_free(ctx);
  if (connection_bio != NULL) BIO_free_all(connection_bio);
  if (certificate_bio != NULL) BIO_free_all(certificate_bio);
  if (x509 != NULL) X509_free(x509);
}

void * __memset_chk(void * dest, int c, size_t len, size_t destlen) {
  printf("!!! %s called\n", __func__);
  return memset(dest, c, len);
}

void * __memcpy_chk(void * dest, const void * src, size_t len, size_t destlen) {
  printf("!!! %s called\n", __func__);
  return memcpy(dest, src, len);
}

int __sprintf_chk(char * str, int flag, size_t strlen, const char * format, ...) {
  printf("!!! %s called\n", __func__);

  // Courtesy https://opensource.apple.com/source/Libc/Libc-498/secure/sprintf_chk.c.auto.html
  va_list arg;
  int done;
  va_start (arg, format);
  if (strlen > (size_t) INT_MAX)
    done = vsprintf (str, format, arg);
  else {
    done = vsnprintf (str, strlen, format, arg);
    // if (done >= 0 && (size_t) done >= strlen) __chk_fail ();
  }
  va_end (arg);
  return done;
}

char * __strcat_chk(char * dest, const char * src, size_t destlen) {
  printf("!!! %s called\n", __func__);
  return strcat(dest, src);
}

long int __fdelt_chk (long int d)
{
  printf("!!! %s called\n", __func__);
  return 0; // idk what this function is supposed to be? can't find any good info on it
}

int
getcontext(ucontext_t *ucp)
{
  printf("!!! %s called\n", __func__);
	return 0;
}

int
setcontext(const ucontext_t *ucp)
{
  printf("!!! %s called\n", __func__);
  return 0;
}

int
swapcontext(ucontext_t *oucp, const ucontext_t *ucp)
{
  printf("!!! %s called\n", __func__);
	return 0;
}

void makecontext(ucontext_t *ucp, void (*func)(), int argc, ...) {
  printf("!!! %s called\n", __func__);
}
