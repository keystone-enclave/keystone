#include "app/eapp_utils.h"
#include "edge/edge_call.h"
#include "app/string.h"
#include "app/syscall.h"

#define OCALL_PRINT_STRING2 3


unsigned long ocall_print_string(char* str);
char* itoa(int value, char* str, int base);  
char* strcat_custom(char* dest, const char* src);  


int main() {
    int x=2, y=6;
    int prod = x * y;
    char message[50];
    char num_str[10];
   
    strcat_custom(message, "le produit de ");  // Ajouter " la somme de " à message

    itoa(x, num_str, 10);  // Convertir 'a' en chaîne
    strcat_custom(message, num_str);  // Ajouter 'a' à message
    
    strcat_custom(message, " et ");  // Ajouter " et "
    
    itoa(y, num_str, 10);  // Convertir 'b' en chaîne
    strcat_custom(message, num_str);  // Ajouter 'b' à message
    
    strcat_custom(message, " est : ");  // Ajouter " est : "
    
    itoa(prod, num_str, 10);  
    strcat_custom(message, num_str);  
    
    // Appeler l'ocall pour afficher le message
    ocall_print_string(message);

    
    EAPP_RETURN(0);
}


unsigned long ocall_print_string(char* str) {
    unsigned long retval;
    ocall(OCALL_PRINT_STRING2, str, strlen(str) + 1, &retval, sizeof(unsigned long));
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