#ifndef __MALLOC_H__
#define __MALLOC_H__

#include <stddef.h>

void* malloc(size_t);
void
free(void*);
void*
realloc(void*, size_t);
void* memalign(size_t, size_t);
void* valloc(size_t);
void* pvalloc(size_t);
void* calloc(size_t, size_t);
void
cfree(void*);
int malloc_trim(size_t);
size_t
malloc_usable_size(void*);
void
malloc_stats(void);
int
mallopt(int, int);
struct mallinfo
mallinfo(void);

typedef struct freelist_entry {
  size_t size;
  struct freelist_entry* next;
} * fle;

#endif
