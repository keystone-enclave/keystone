#ifndef _KEYSTONE_H_
#define _KEYSTONE_H_

#include <stddef.h>

#define KEYSTONE_DEV_PATH "/dev/keystone_enclave"

// SBI
#define KEYSTONE_CREATE_ENCLAVE   101
#define KEYSTONE_DESTROY_ENCLAVE  102

// ABI
#define KEYSTONE_GET_FIELD          1000
#define KEYSTONE_COPY_FROM_ENCLAVE  1004
#define KEYSTONE_COPY_TO_ENCLAVE    1005

typedef enum {
  KEYSTONE_ERROR=-1,
  KEYSTONE_SUCCESS=0,
} keystone_status_t;

class Keystone
{
private:
  int eid;
public:
  Keystone();
  ~Keystone();
  keystone_status_t init();
  keystone_status_t copy_from_encl(void* ptr, size_t size);
  keystone_status_t copy_to_encl(void* ptr, size_t size);
};


#endif
