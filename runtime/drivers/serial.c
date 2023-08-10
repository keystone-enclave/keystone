/*
* SPDX-License-Identifier: BSD-2-Clause
*
* Copyright (c) 2019 Western Digital Corporation or its affiliates.
*
* Authors:
*   Anup Patel <anup.patel@wdc.com>
*/

#include "drivers/drivers.h"
#include "uaccess.h"

#include "call/syscall.h"

#include <stdbool.h>

#include "mm/mm.h"
#include "mm/vm.h"

/* clang-format off */

#define UART_RBR_OFFSET		0	/* In:  Recieve Buffer Register */
#define UART_THR_OFFSET		0	/* Out: Transmitter Holding Register */
#define UART_DLL_OFFSET		0	/* Out: Divisor Latch Low */
#define UART_IER_OFFSET		1	/* I/O: Interrupt Enable Register */
#define UART_DLM_OFFSET		1	/* Out: Divisor Latch High */
#define UART_FCR_OFFSET		2	/* Out: FIFO Control Register */
#define UART_IIR_OFFSET		2	/* I/O: Interrupt Identification Register */
#define UART_LCR_OFFSET		3	/* Out: Line Control Register */
#define UART_MCR_OFFSET		4	/* Out: Modem Control Register */
#define UART_LSR_OFFSET		5	/* In:  Line Status Register */
#define UART_MSR_OFFSET		6	/* In:  Modem Status Register */
#define UART_SCR_OFFSET		7	/* I/O: Scratch Register */
#define UART_MDR1_OFFSET	8	/* I/O:  Mode Register */

#define UART_LSR_FIFOE		0x80	/* Fifo error */
#define UART_LSR_TEMT		0x40	/* Transmitter empty */
#define UART_LSR_THRE		0x20	/* Transmit-hold-register empty */
#define UART_LSR_BI		0x10	/* Break interrupt indicator */
#define UART_LSR_FE		0x08	/* Frame error indicator */
#define UART_LSR_PE		0x04	/* Parity error indicator */
#define UART_LSR_OE		0x02	/* Overrun error indicator */
#define UART_LSR_DR		0x01	/* Receiver data ready */
#define UART_LSR_BRK_ERROR_BITS	0x1E	/* BI, FE, PE, OE bits */

static inline void __raw_writeb(uint8_t val, volatile void *addr)
{
	__asm__ volatile("sb %0, 0(%1)" : : "r"(val), "r"(addr));
}

static inline void __raw_writew(uint16_t val, volatile void *addr)
{
	__asm__ volatile("sh %0, 0(%1)" : : "r"(val), "r"(addr));
}

static inline void __raw_writel(uint32_t val, volatile void *addr)
{
	__asm__ volatile("sw %0, 0(%1)" : : "r"(val), "r"(addr));
}

#if __riscv_xlen != 32
static inline void __raw_writeq(uint64_t val, volatile void *addr)
{
	__asm__ volatile("sd %0, 0(%1)" : : "r"(val), "r"(addr));
}
#endif

static inline uint8_t __raw_readb(const volatile void *addr)
{
	uint8_t val;

	__asm__ volatile("lb %0, 0(%1)" : "=r"(val) : "r"(addr));
	return val;
}

static inline uint16_t __raw_readw(const volatile void *addr)
{
	uint16_t val;

	__asm__ volatile("lh %0, 0(%1)" : "=r"(val) : "r"(addr));
	return val;
}

static inline uint32_t __raw_readl(const volatile void *addr)
{
	uint32_t val;

	__asm__ volatile("lw %0, 0(%1)" : "=r"(val) : "r"(addr));
	return val;
}

#if __riscv_xlen != 32
static inline uint64_t __raw_readq(const volatile void *addr)
{
	uint64_t val;

	__asm__ volatile("ld %0, 0(%1)" : "=r"(val) : "r"(addr));
	return val;
}
#endif

#define __io_br()	do {} while (0)
#define __io_ar()	__asm__ __volatile__ ("fence i,r" : : : "memory");
#define __io_bw()	__asm__ __volatile__ ("fence w,o" : : : "memory");
#define __io_aw()	do {} while (0)

#define readb(c)	({ uint8_t  __v; __io_br(); __v = __raw_readb(c); __io_ar(); __v; })
#define readw(c)	({ uint16_t __v; __io_br(); __v = __raw_readw(c); __io_ar(); __v; })
#define readl(c)	({ uint32_t __v; __io_br(); __v = __raw_readl(c); __io_ar(); __v; })

#define writeb(v,c)	({ __io_bw(); __raw_writeb((v),(c)); __io_aw(); })
#define writew(v,c)	({ __io_bw(); __raw_writew((v),(c)); __io_aw(); })
#define writel(v,c)	({ __io_bw(); __raw_writel((v),(c)); __io_aw(); })

/* clang-format on */

static bool configured = false;
static volatile void *uart8250_base;
static uint32_t uart8250_in_freq;
static uint32_t uart8250_baudrate;
static uint32_t uart8250_reg_width;
static uint32_t uart8250_reg_shift;

static uint32_t get_reg(uint32_t num)
{
  uint32_t offset = num << uart8250_reg_shift;

 if (uart8250_reg_width == 1)
   return readb(uart8250_base + offset);
 else if (uart8250_reg_width == 2)
   return readw(uart8250_base + offset);
 else
   return readl(uart8250_base + offset);
}

static void set_reg(uint32_t num, uint32_t val)
{
  uint32_t offset = num << uart8250_reg_shift;

 if (uart8250_reg_width == 1)
   writeb(val, uart8250_base + offset);
 else if (uart8250_reg_width == 2)
   writew(val, uart8250_base + offset);
 else
   writel(val, uart8250_base + offset);
}

void uart8250_configure() {
  uint16_t bdiv;
  bdiv = uart8250_in_freq / (16 * uart8250_baudrate);

  /* Map memory */
  uart8250_base = (void *) map_anywhere_with_dynamic_page_table((uintptr_t) uart8250_base, 0x1000);

  /* Disable all interrupts */
  set_reg(UART_IER_OFFSET, 0x00);
  /* Enable DLAB */
  set_reg(UART_LCR_OFFSET, 0x80);

  if (bdiv) {
    /* Set divisor low byte */
    set_reg(UART_DLL_OFFSET, bdiv & 0xff);
    /* Set divisor high byte */
    set_reg(UART_DLM_OFFSET, (bdiv >> 8) & 0xff);
  }

  /* 8 bits, no parity, one stop bit */
  set_reg(UART_LCR_OFFSET, 0x03);
  /* Enable FIFO */
  set_reg(UART_FCR_OFFSET, 0x01);
  /* No modem control DTR RTS */
  set_reg(UART_MCR_OFFSET, 0x00);
  /* Clear line status */
  get_reg(UART_LSR_OFFSET);
  /* Read receive buffer */
  get_reg(UART_RBR_OFFSET);
  /* Set scratchpad */
  set_reg(UART_SCR_OFFSET, 0x00);
}

void uart8250_putc(char ch)
{
  if(!configured) {
    uart8250_configure();
    configured = true;
  }

 while ((get_reg(UART_LSR_OFFSET) & UART_LSR_THRE) == 0)
   ;

 set_reg(UART_THR_OFFSET, ch);
}

int uart8250_getc(void)
{
  if(!configured) {
    uart8250_configure();
    configured = true;
  }

 if (get_reg(UART_LSR_OFFSET) & UART_LSR_DR)
   return get_reg(UART_RBR_OFFSET);
 return -1;
}

/**
 *
 * 
 *  EAPP-DRIVER INTERFACE DEFINITION
 * 
 * 
 */
#define UART8250_BUF_SIZE 512
char uart8250_buf[UART8250_BUF_SIZE];

int uart8250_init(void);
size_t uart8250_write(void* buf, size_t len);
size_t uart8250_read(void* buf, size_t len);

driver_instance uart8250_driver = {
  .name = "uart8250",
  .init = uart8250_init,
  .fini = NULL,
  .read = uart8250_read,
  .write = uart8250_write
};

int uart8250_init() {
  /*
   * initialize serial:
      uart@10001000 {
        interrupts = <0x09>;
        interrupt-parent = <0x03>;
        clock-frequency = "\08@";
        compatible = "ns16550a";
        status = "disabled";
        reg = <0x00 0x10001000 0x00 0x100>;
        secure-status = "okay";
      };
  */

  // constants for now, based on above
  uart8250_base      = (volatile void*)0x10001000;
  uart8250_reg_shift = 0x0;
  uart8250_reg_width = 0x1;
  uart8250_in_freq   = 0x384000;
  uart8250_baudrate  = 0x1c200;

  print_strace("!!! uart8250_init completed\n");
  return 0;
}

size_t uart8250_write(void* buf, size_t len) {
  size_t to_write = len >= UART8250_BUF_SIZE ? UART8250_BUF_SIZE : len;
  copy_from_user(uart8250_buf, buf, to_write);
  for (size_t i = 0; i < to_write; i += 1) {
    uart8250_putc(uart8250_buf[i]);
  }
  return to_write;
}

size_t uart8250_read(void* buf, size_t len) {
  size_t to_read = len >= UART8250_BUF_SIZE ? UART8250_BUF_SIZE : len;

  size_t i = 0;
  int c = 0;
  for (; i < to_read && c != -1; i += 1) {
    int c = uart8250_getc();
    if (c != -1) {
      uart8250_buf[i] = (char)c;
    }
  }
  copy_to_user(buf, uart8250_buf, i);
  return i;
}
