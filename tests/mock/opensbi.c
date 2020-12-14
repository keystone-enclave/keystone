#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <sbi/riscv_locks.h>
#include <sbi/sbi_string.h>
#include <sbi/sbi_ecall.h>

extern void mock_assert(const int result, const char* const expression,
                            const char * const file, const int line);

void __wrap_sbi_printf(const char* s, ...)
{
  va_list args;
  va_start (args, s);
  printf (s, args);
  va_end (args);
  return;
}

void __wrap_sbi_hart_hang(uint16_t code)
{
  mock_assert(0, "poweroff", __FILE__, __LINE__);
}

void __wrap_spin_lock(spinlock_t* lock)
{
  return;
}

void __wrap_spin_unlock(spinlock_t* lock)
{
  return;
}

void __wrap_spin_trylock(spinlock_t* lock)
{
  return;
}

void* __wrap_sbi_memcpy(void *dest, const void *src, size_t count)
{
  char *temp1 = dest;
  const char *temp2 = src;

  while (count > 0) {
    *temp1++ = *temp2++;
    count --;
  }

  return dest;
}

void* __wrap_sbi_memset(void *s, int c, size_t count)
{
  char *temp = s;

  while(count > 0) {
    count--;
    *temp++ = c;
  }

  return s;
}

void __wrap_trap_vector_enclave(){
  return;
}

void __wrap__trap_handler(){
  return;
}

int __wrap_sbi_ecall_register_extension(struct sbi_ecall_extension *ext)
{
  return 0;
}

void* __wrap_ecall_keystone_enclave;
