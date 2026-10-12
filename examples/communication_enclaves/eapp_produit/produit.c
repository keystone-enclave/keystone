#include "app/eapp_utils.h"
#include "app/string.h"
#include "edge/edge_call.h"
#include "app/syscall.h"

#define OCALL_GET_RESULT 2
#define OCALL_PRINT_STRING 3

unsigned long ocall_print_string(char* str);
unsigned long ocall_get_result(int* buf, size_t buf_size);
char* itoa(int value, char* str, int base);
char* strcat_custom(char* dest, const char* src);    

int main() {
    int resultat;
    char message[50];
    char num_str[10];

    ocall_get_result(&resultat, sizeof(resultat));// Lire le resultat depuis l'autre enclave
    int produit = resultat * 5;
    strcat_custom(message, "le resultat est: ");
    itoa(produit, num_str, 10);  // Convertir 'resultat' en chaîne
    strcat_custom(message, num_str);  // Ajouter 'resultat' à message
    ocall_print_string(message);

    EAPP_RETURN(0);
}

unsigned long ocall_print_string(char* str) {
    unsigned long retval;
    ocall(OCALL_PRINT_STRING, str, strlen(str) + 1, &retval, sizeof(unsigned long));
    return retval;
}

unsigned long ocall_get_result(int* buf, size_t buf_size) {
    unsigned long retval;
    ocall(OCALL_GET_RESULT, NULL, 0, buf, buf_size);
    return retval;
}

char* itoa(int value, char* str, int base) {
    int i = 0;
    int isNegative = 0;

    if (value == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    if (value < 0 && base == 10) {
        isNegative = 1;
        value = -value;
    }

    while (value != 0) {
        int rem = value % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        value = value / base;
    }

    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }

    return str;
}

char* strcat_custom(char* dest, const char* src) {
    char* ptr = dest + strlen(dest);
    while (*src) {
        *ptr++ = *src++;
    }
    *ptr = '\0';
    return dest;
}