#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <vta/driver.h>

#include <thread>

#define FIELDS(x)  (x * sizeof(uint32_t))

#define FETCH_PAD0  (VTA_FETCH_INSN_COUNT_OFFSET - 0 - FIELDS(1))
#define FETCH_PAD1  (VTA_FETCH_INSN_ADDR_OFFSET - VTA_FETCH_INSN_COUNT_OFFSET - FIELDS(1))
#define FETCH_PAD2  (VTA_LOAD_ADDR - (VTA_FETCH_ADDR + VTA_FETCH_INSN_ADDR_OFFSET + FIELDS(1)))

#define LOAD_PAD0   (VTA_LOAD_INP_ADDR_OFFSET - 0 - FIELDS(1))
#define LOAD_PAD1   (VTA_LOAD_WGT_ADDR_OFFSET - VTA_LOAD_INP_ADDR_OFFSET - FIELDS(1))
#define LOAD_PAD2   (VTA_COMPUTE_ADDR - (VTA_LOAD_ADDR + VTA_LOAD_WGT_ADDR_OFFSET + FIELDS(1)))

#define COMP_PAD0   (VTA_COMPUTE_DONE_WR_OFFSET - 0 - FIELDS(1))
#define COMP_PAD1   (VTA_COMPUTE_DONE_RD_OFFSET - VTA_COMPUTE_DONE_WR_OFFSET - FIELDS(1))
#define COMP_PAD2   (VTA_COMPUTE_UOP_ADDR_OFFSET - VTA_COMPUTE_DONE_RD_OFFSET - FIELDS(1))
#define COMP_PAD3   (VTA_COMPUTE_BIAS_ADDR_OFFSET - VTA_COMPUTE_UOP_ADDR_OFFSET - FIELDS(1))
#define COMP_PAD4   (VTA_STORE_ADDR - (VTA_COMPUTE_ADDR + VTA_COMPUTE_BIAS_ADDR_OFFSET + FIELDS(1)))

#define STORE_PAD0  (VTA_STORE_OUT_ADDR_OFFSET - 0 - FIELDS(1))

/*! \brief VTA configuration register start value */
#define VTA_START 0b01
/*! \brief VTA configuration register auto-restart value */
#define VTA_AUTORESTART 0x81
/*! \brief VTA configuration register done value */
#define VTA_DONE 0b10

//typedef struct _vtaregs
//{
//  /* Fetch unit */
//  volatile uint32_t fetch_cmd;
//  uint8_t __fetch_pad0[FETCH_PAD0];
//  volatile uint32_t insn_count;
//  uint8_t __fetch_pad1[FETCH_PAD1];
//  volatile uint32_t insn_addr;
//  uint8_t __fetch_pad2[FETCH_PAD2];
//
//  /* Load unit */
//  volatile uint32_t load_cmd;
//  uint8_t __load_pad0[LOAD_PAD0];
//  volatile uint32_t inp_addr;
//  uint8_t __load_pad1[LOAD_PAD1];
//  volatile uint32_t wgt_addr;
//  uint8_t __load_pad2[LOAD_PAD2];
//
//  /* Compute unit */
//  volatile uint32_t compute_cmd;
//  uint8_t __comp_pad0[COMP_PAD0];
//  volatile uint32_t wr_done;
//  uint8_t __comp_pad1[COMP_PAD1];
//  volatile uint32_t rd_done;
//  uint8_t __comp_pad2[COMP_PAD2];
//  volatile uint32_t uop_addr;
//  uint8_t __comp_pad3[COMP_PAD3];
//  volatile uint32_t bias_addr;
//  uint8_t __comp_pad4[COMP_PAD4];
//
//  /* Store unit */
//  volatile uint32_t store_cmd;
//  uint8_t __store_pad0[STORE_PAD0];
//  volatile uint32_t out_addr;
//} vta_t;

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

static int get_uio_device(const char * id)
{
  FILE *fp;
  int i, check;
  char file_id[256];
  char sysfs_path[256];

  for (i = 0; i < 32; i++) {
    snprintf(sysfs_path, sizeof(sysfs_path), "/sys/class/uio/uio%d/name", i);
    fp = fopen(sysfs_path, "r");
    if (fp == NULL)
      break;
    check = fscanf(fp, "%32s", file_id);
    (void) check;
    if (strncmp(file_id, id, sizeof(file_id)) == 0) {
      return i;
    }
  }
  return -1;
}

volatile bool loop = true;

class VTADevice {
 public:
  VTADevice() {
    char devname[32];
    int devid = get_uio_device("vta@60000000");
    if(devid < 0) {
      fprintf(stderr, "VTADevice() could not find UIO device\n");
      throw;
    }

    snprintf(devname, sizeof(devname), "/dev/uio%d", devid);
    fd = open(devname, O_RDWR);
    if(fd < 0) {
      fprintf(stderr, "VTADevice() could not open UIO device\n");
      throw;
    }

    device = (vta_t *) mmap(NULL, 0x8000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(device == MAP_FAILED) {
      fprintf(stderr, "VTADevice() could not map UIO device\n");
      close(fd);
      throw;
    }
  }

  ~VTADevice() {
    munmap(device, 0x8000);
    close(fd);
  }

  int Run(vta_phy_addr_t insn_phy_addr,
          uint32_t insn_count,
          uint32_t wait_cycles) {
    int i;
    char input[512];
    fprintf(stderr, "VTADevice() run\n");


    // Configure
//    fprintf(stderr, "Hit enter to write insn_count\n");
//    fgets(input, sizeof(input), stdin);
    device->insn_count = insn_count;

//    fprintf(stderr, "Hit enter to write insn_phy_addr\n");
//    fgets(input, sizeof(input), stdin);
    device->insn_addr = insn_phy_addr;
//    device->inp_addr = 0;
//    device->wgt_addr = 0;
//    device->uop_addr = 0;
////    device->bias_addr = 0;
//    device->out_addr = 0;

    // Kick off computation
//    fprintf(stderr, "Hit enter to start\n");
//    fgets(input, sizeof(input), stdin);
    device->fetch_cmd = VTA_START;
    device->fetch_cmd = 0;
    device->fetch_cmd = VTA_START;

/*    device->load_cmd = VTA_AUTORESTART;
    device->compute_cmd = VTA_AUTORESTART;
    device->store_cmd = VTA_AUTORESTART;*/

    // Allow device to respond
    struct timespec ts = { .tv_sec = 0, .tv_nsec = 1000 };
    nanosleep(&ts, &ts);

    // Loop until the VTA is done
    unsigned t, flag = 0;
    for (t = 0; t < wait_cycles; ++t) {
//      fprintf(stderr, "Hit enter to read status\n");
//      fgets(input, sizeof(input), stdin);
      flag = device->fetch_cmd & 0x2;
      if (flag == VTA_DONE) break;
      std::this_thread::yield();
    }
    // Report error if timeout
    return t < wait_cycles ? 0 : 1;
  }

 private:
  int fd;
  vta_t *device;
};

VTADeviceHandle VTADeviceAlloc() {
  fprintf(stderr, "VTADeviceAlloc\n");
  return new VTADevice();
}

void VTADeviceFree(VTADeviceHandle handle) {
  fprintf(stderr, "VTADeviceFree\n");
  delete static_cast<VTADevice*>(handle);
}

int VTADeviceRun(VTADeviceHandle handle,
                 vta_phy_addr_t insn_phy_addr,
                 uint32_t insn_count,
                 uint32_t wait_cycles) {
  fprintf(stderr, "VTADeviceRun(insn_phy_addr = %lx, insn_count = %x, wait_cycles = %x)\n",
          (unsigned long) insn_phy_addr, insn_count, wait_cycles);

  return static_cast<VTADevice*>(handle)->Run(
      insn_phy_addr, insn_count, wait_cycles);
}
