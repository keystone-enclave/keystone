
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/user.h>
#include <unistd.h>
#include <vta/driver.h>

#define UDMABUF_POOL "udmabuf-ddr-nc0"

static volatile bool loop = true;
static bool dma_initialized = false;
uint64_t dma_base;
static uint8_t *dma_mem;

struct dma_allocation {
  uint64_t base, offs;
  size_t size;

  struct dma_allocation *next;
};

struct dma_allocation *free_list;

int read_field(const char *field, const char *format, void *data) {
  char filename[256];
  int check;

  snprintf(filename, sizeof(filename),
           "/sys/class/u-dma-buf/" UDMABUF_POOL "/%s", field);
  FILE *fp = fopen(filename, "r");
  if(!fp) {
    fprintf(stderr, "could not find " UDMABUF_POOL " physical address\n");
    return 0;
  }

  check = fscanf(fp, format, data);
  (void) check;
  fclose(fp);
  return 1;
}

int ensure_initialized() {
  int fd;
  uint64_t base;
  size_t size;

//  while(loop);
//  loop = 0;

  if(dma_initialized) {
    return 1;
  }

  // Read necessary fields
  if(!read_field("phys_addr", "%lx", &base)) {
    fprintf(stderr, "ensure_initialized could not get base\n");
    return 0;
  }

  if(!read_field("size", "%lu", &size)) {
    fprintf(stderr, "ensure_initialized could not get size\n");
    return 0;
  }

  fd = open("/dev/" UDMABUF_POOL, O_RDWR);
  if(fd < 0) {
    fprintf(stderr, "ensure_initialized could not open " UDMABUF_POOL "\n");
    return 0;
  }

  dma_mem = (uint8_t *) mmap(nullptr, size, PROT_READ | PROT_WRITE,
                 MAP_SHARED, fd, 0);
  if(dma_mem == MAP_FAILED) {
    fprintf(stderr, "ensure_initialized could not map " UDMABUF_POOL "\n");
    close(fd);
    return 0;
  }

  // Seed our allocator with the first chunk
  dma_base = base;

  free_list = new struct dma_allocation;
  free_list->base = base;
  free_list->offs = 0;
  free_list->size = size;
  free_list->next = nullptr;
  dma_initialized = true;
  return 1;
}

#define __round_mask(x, y) ((__typeof__(x))((y)-1))
#define round_up(x, y) ((((x)-1) | __round_mask(x, y))+1)

void* VTAMemAlloc(size_t size, int cached) {
  struct dma_allocation *curr, *prev = nullptr;
  void *result = nullptr;

  fprintf(stderr, "VTAMemAlloc size=%lx, cached=%i\n", size, cached);
  if(!ensure_initialized())
    return nullptr;

  // Size should be page aligned
  size = round_up(size, 0x1000);

  // Traverse the freelist to see if we can find a chunk
  curr = free_list;
  while(curr != nullptr) {
    if(size <= curr->size) {
      // Our allocation fits in this chunk
      result = dma_mem + curr->offs;
      if(size == curr->size) {
        if(prev) {
          prev->next = curr->next;
        } else {
          free_list = curr->next;
        }

        free(curr);
        curr = nullptr;
      } else {
        // Carve out the requested chunk
        curr->size -= size;
        curr->base += size;
        curr->offs += size;
      }

      fprintf(stderr, "\tReturning %p\n", result);
      break;
    }

    prev = curr;
    curr = curr->next;
  }

  return result;
}

void VTAMemFree(void* buf) {
  if(!ensure_initialized())
    return;

  fprintf(stderr, "VTAMemFree currently unimplemented\n");
}

vta_phy_addr_t VTAMemGetPhyAddr(void* buf) {
  if(!ensure_initialized())
    return 0;

  vta_phy_addr_t res = ((uint64_t) buf - (uint64_t) dma_mem) + dma_base;
  fprintf(stderr, "VTAMemGetPhyAddr buf = %p -> %x\n", buf, res);
  return res;
}

void VTAMemCopyFromHost(void* dst, const void* src, size_t size) {
  int i;
  if(!ensure_initialized())
    return;

  fprintf(stderr, "VTAMemCopyFromHost dst=%p src=%p size=%lx\n", dst, src, size);

//  for(i = 0; i < size; i++) {
//    if(i % 32 == 0) {
//      fprintf(stderr,"\n(%lx)>", (uint64_t) src + i);
//    }
//
//    if(i % 8 == 0) {
//      fprintf(stderr, " ");
//    }
//
//    fprintf(stderr, "%02x", ((char *) src)[i]);
//  }
//
//  fprintf(stderr, "\n");
  memcpy(dst, src, size);
}

void VTAMemCopyToHost(void* dst, const void* src, size_t size) {
  int i;
  if(!ensure_initialized())
    return;

//  fprintf(stderr, "VTAMemCopyToHost dst=%p src=%p size=%lx\n", dst, src, size);
//  for(i = 0; i < size; i++) {
//    if(i % 32 == 0) {
//      fprintf(stderr,"\n(%lx)>", (uint64_t) src + i);
//    }
//
//    if(i % 8 == 0) {
//      fprintf(stderr, " ");
//    }
//
//    fprintf(stderr, "%02x", ((char *) src)[i]);
//  }
//
//  fprintf(stderr, "\n");
  memcpy(dst, src, size);
}

void VTAFlushCache(void* vir_addr, vta_phy_addr_t phy_addr, int size) {
  fprintf(stderr, "VTAFlushCache(vir_addr = %p, phy_addr = %lx, size = %x)\n",
          vir_addr, (unsigned long) phy_addr, size);
}

void VTAInvalidateCache(void* vir_addr, vta_phy_addr_t phy_addr, int size) {
  fprintf(stderr, "VTAInvalidateCache(vir_addr = %p, phy_addr = %lx, size = %x)\n",
          vir_addr, (unsigned long) phy_addr, size);
}