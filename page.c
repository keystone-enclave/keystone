#include "page.h"

int init_encl_pgtable(int level, pte_t* tb, uintptr_t base, uintptr_t size, uintptr_t utbase, uintptr_t utsize){
  pte_t* walk;
  int ret = 0;
  int i=0;
  
  for (walk=tb; walk < tb + (RISCV_PGSIZE/sizeof(pte_t)) ; walk += 1)
  {
    if(*walk == 0)
      continue;

    pte_t pte = *walk;
    uintptr_t phys_addr = (pte >> PTE_PPN_SHIFT) << RISCV_PGSHIFT;
    if(!((phys_addr >= base && phys_addr < base + size) ||
        (phys_addr >= utbase && phys_addr< utbase + utsize)))
    {
      *walk = 0;
      continue;
    }
    if(level > 1)
    {
      ret |= init_encl_pgtable(level - 1, (pte_t*) phys_addr, base, size, utbase, utsize);
    }
  }
  return ret;
}
/* uncomment this function for debugging 
void pte_to_str(pte_t pte, char* buf)
{
  int i;
  for(i=0; i<8; i++)
    buf[i] = '-';
  buf[8] = '\0';

  if( pte & PTE_D )
    buf[0] = 'D';
  if( pte & PTE_A )
    buf[1] = 'A';
  if( pte & PTE_G )
    buf[2] = 'G';
  if( pte & PTE_U )
    buf[3] = 'U';
  if( pte & PTE_X )
    buf[4] = 'X';
  if( pte & PTE_W )
    buf[5] = 'W';
  if( pte & PTE_R )
    buf[6] = 'R';
  if( pte & PTE_V )
    buf[7] = 'V';
  return;
}

int print_pgtable(int level, pte_t* tb, uintptr_t vaddr)
{
  pte_t* walk;
  int ret = 0;
  int i=0;
  
  for (walk=tb, i=0; walk < tb + (RISCV_PGSIZE/sizeof(pte_t)) ; walk += 1, i++)
  {
    if(*walk == 0)
      continue;

    pte_t pte = *walk;
    uintptr_t phys_addr = (pte >> PTE_PPN_SHIFT) << RISCV_PGSHIFT;
   
    if(level == 1)
    {
      char buf[10];
      pte_to_str(pte, buf);
      printm("[pgtable] level:%d, base: 0x%lx, i:%d, pte: 0x%lx (vaddr: 0x%lx : %s)\r\n", level, tb, i, pte, ((vaddr << 9) | (i&0x1ff))<<12, buf);
    }
    else
    {
      char buf[10];
      pte_to_str(pte, buf);
      printm("[pgtable] level:%d, base: 0x%lx, i:%d, pte: 0x%lx (%s)\r\n", level, tb, i, pte, buf);
    }

    if(level > 1)
    {
      if(level == 3 && (i&0x100))
        vaddr = 0xffffffffffffffffUL;
      ret |= print_pgtable(level - 1, (pte_t*) phys_addr, (vaddr << 9) | (i&0x1ff));
    }
  }
  return ret;
} */
