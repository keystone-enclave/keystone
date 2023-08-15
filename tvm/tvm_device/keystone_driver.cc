#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <vta/driver.h>

#include <thread>

#include <app/syscall.h>

/*! \brief VTA configuration register done value */
#define VTA_DONE 0b10

typedef struct _vtaregs
{
  volatile uint32_t fetch_cmd;
  volatile uint32_t events;
  volatile uint32_t insn_count;
  volatile uint32_t insn_addr;
  volatile uint32_t uop_addr;
  volatile uint32_t inp_addr;
  volatile uint32_t wgt_addr;
  volatile uint32_t acc_addr;
  volatile uint32_t out_addr;
} vta_t;

static const char VTA_DEVICE[] = "vta@60000000";
static const char MEM_DEVICE[] = "cma@C0000000";

class VTADevice {
 public:
  VTADevice() {
    int ret = claim_mmio(VTA_DEVICE, sizeof(VTA_DEVICE));
    if(ret < 0)
      throw;

    fd = openat(-2, "vta", 0, 0);
    if(fd < 0) {
      throw;
    }
  }

  ~VTADevice() {
    close(fd);
    release_mmio(VTA_DEVICE, sizeof(VTA_DEVICE));
  }

  int Run(vta_phy_addr_t insn_phy_addr,
          uint32_t insn_count,
          uint32_t wait_cycles) {
    size_t ret;

    uint32_t args[2] = {
      insn_phy_addr,
      insn_count
    }, t, flag;

    ret = write(fd, args, sizeof(args));
    if(ret != 0) {
      throw;
    }

    struct timespec ts = { .tv_sec = 0, .tv_nsec = 1000 };
    nanosleep(&ts, &ts);

    for(t = 0; t < wait_cycles; ++t) {
      ret = read(fd, &flag, sizeof(uint32_t));
      if(ret != 0) {
        throw;
      }

      if (flag == VTA_DONE) break;
    }

    return t < wait_cycles ? 0 : 1;
  }

 private:
  int fd;
};

VTADeviceHandle VTADeviceAlloc() {
  return new VTADevice();
}

void VTADeviceFree(VTADeviceHandle handle) {
  delete static_cast<VTADevice*>(handle);
}

int VTADeviceRun(VTADeviceHandle handle,
                 vta_phy_addr_t insn_phy_addr,
                 uint32_t insn_count,
                 uint32_t wait_cycles) {
  return static_cast<VTADevice*>(handle)->Run(
      insn_phy_addr, insn_count, wait_cycles);
}

static bool dma_initialized = false;
uint64_t dma_base;
static uint8_t *dma_mem;

// Typically, this would not actually be physically contiguous memory.
// However, this file ends up being linked into a static library, which
// ends up being linked into a keystone enclave, which is always loaded
// with a linear page table mapping!

//__attribute__ ((aligned(4096)))
//static uint8_t dma_mem[128 * 1024 * 1024];

struct dma_allocation {
  uint64_t base, offs;
  size_t size;

  struct dma_allocation *next;
};

struct dma_allocation *free_list;

int ensure_initialized() {
  if(dma_initialized) {
    return 1;
  }

  int ret;
  uint64_t base = 0xC0000000;
  size_t size = 0x10000000;
  uintptr_t mapped;

  ret = claim_mmio(MEM_DEVICE, sizeof(MEM_DEVICE));
  if(ret < 0) {
    throw;
  }

  mapped = map(base, size, base);
  if(mapped != base) {
    throw;
  }

  // Seed our allocator with the first chunk
  dma_base = base;
  dma_mem = (uint8_t *) base;

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
}

vta_phy_addr_t VTAMemGetPhyAddr(void* buf) {
  if(!ensure_initialized())
    return 0;

  vta_phy_addr_t res = ((uint64_t) buf - (uint64_t) dma_mem) + dma_base;
  return res;
}

void VTAMemCopyFromHost(void* dst, const void* src, size_t size) {
  if(!ensure_initialized())
    return;

  memcpy(dst, src, size);
}

void VTAMemCopyToHost(void* dst, const void* src, size_t size) {
  if(!ensure_initialized())
    return;

  memcpy(dst, src, size);
}

void VTAFlushCache(void* vir_addr, vta_phy_addr_t phy_addr, int size) {
  return;
}

void VTAInvalidateCache(void* vir_addr, vta_phy_addr_t phy_addr, int size) {
  return;
}
