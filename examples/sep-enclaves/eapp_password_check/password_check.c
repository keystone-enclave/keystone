#include "app/eapp_utils.h"
#include "app/string.h"
#include "edge/edge_call.h"
#include "app/syscall.h"

#define  OCALL_READ_PASSWORD 1
#define  OCALL_PRINT_STRING 2
#define  SECRET_PASSWORD "Oussama"


unsigned long ocall_print_string(char* str);
unsigned long ocall_read_password(char* buf, size_t buf_size);
               

int my_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (*s1 == *s2) ? 1 : 0;
}

int main() {
    char input_password[64];

    // Lire le mot de passe depuis l'extérieur
    ocall_read_password(input_password, sizeof(input_password));

    // Comparer avec le mot de passe attendu
    if (my_strcmp(input_password, SECRET_PASSWORD) == 1) {
        ocall_print_string("Mot de passe valide.");
    } else {
       ocall_print_string("Mot de passe invalide.");
    }

    EAPP_RETURN(0);
}

unsigned long ocall_print_string(char* str) {
    unsigned long retval;
    ocall(OCALL_PRINT_STRING, str, strlen(str) + 1, &retval, sizeof(unsigned long));
    return retval;
}

unsigned long ocall_read_password(char* buf, size_t buf_size) {
    unsigned long retval;
    ocall(OCALL_READ_PASSWORD, NULL, 0, buf, buf_size);
    return retval;
}
