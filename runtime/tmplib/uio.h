#ifndef _UIO_H_
#define _UIO_H_

/* See Linux's include/uapi/linux/uio.h */
struct iovec{
  void* buffer_start;
  size_t len;
}

#endif /* _UIO_H_ */
