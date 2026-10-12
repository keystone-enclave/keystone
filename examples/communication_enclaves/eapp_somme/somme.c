#include "app/eapp_utils.h"
#include "edge/edge_call.h"
#include "app/string.h"
#include "app/syscall.h"

#define OCALL_SEND_RESULT 1


unsigned long ocall_send_result(int result);
char* itoa(int value, char* str, int base);  
char* strcat_custom(char* dest, const char* src);  


int main() {
    int x=2, y=6;
    int sum = x + y;
    ocall_send_result(sum);  
    EAPP_RETURN(0);
}


unsigned long ocall_send_result(int result) {
    unsigned long retval;
    ocall(OCALL_SEND_RESULT, &result, sizeof(result), &retval, sizeof(unsigned long));
    return retval;
}

