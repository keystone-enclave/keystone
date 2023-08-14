#include "drivers/drivers.h"
#include "mm/mm.h"
#include "uaccess.h"

/*! \brief VTA configuration register start value */
#define VTA_START 0b01
/*! \brief VTA configuration register auto-restart value */
#define VTA_AUTORESTART 0x81
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

vta_t *regs = NULL;

int vta_init(void) {
  regs = (vta_t *) map_anywhere_with_dynamic_page_table((uintptr_t) 0x60000000, 0x1000);
  if(regs) {
    return 0;
  } else return -1;
}

int vta_fini(void) {
  // don't unmap, this causes an assertion failure :/

//  unmap_with_any_page_table((uintptr_t )regs, 0x1000);
  return 0;
}

size_t vta_read(void *ptr, size_t len) {
  // We'll treat read as a polling function for "doneness"
  uint32_t flag;

  if(len != sizeof(uint32_t)) {
    return -1;
  }

  flag = regs->fetch_cmd & 0x2;
  copy_to_user(ptr, &flag, len);
  return 0;
}

size_t vta_write(void *ptr, size_t len) {
  // We'll treat write as the start function
  uint32_t args[2], insn_count, insn_phy_addr;

  if(len != 2 * sizeof(uint32_t)) {
    return -1;
  }

  copy_from_user(args, ptr, len);
  insn_count = args[1];
  insn_phy_addr = args[0];

  regs->insn_count = insn_count;
  regs->insn_addr = insn_phy_addr;

  regs->fetch_cmd = VTA_START;
  regs->fetch_cmd = 0;
  regs->fetch_cmd = VTA_START;
  return 0;
}

driver_instance vta_driver = {
    .name = "vta",
    .init = vta_init, .fini = vta_fini,
    .read = vta_read, .write = vta_write
};

