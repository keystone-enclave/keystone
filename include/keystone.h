#ifndef _KEYSTONE_H_
#define _KEYSTONE_H_

#include <stddef.h>

#define KEYSTONE_DEV_PATH "/dev/keystone_enclave"

typedef enum {
  KEYSTONE_ERROR=-1,
  KEYSTONE_SUCCESS,
} keystone_status_t;

class Keystone
{
private:
  int eid;
  int fd;
public:
  Keystone();
  ~Keystone();
  keystone_status_t init();
  keystone_status_t destroy();
  keystone_status_t copy_from_encl(void* ptr, size_t size);
  keystone_status_t copy_to_encl(void* ptr, size_t size);
};


#endif
