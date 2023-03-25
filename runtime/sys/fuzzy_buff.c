#include "sys/fuzzy_buff.h"
#include "string.h"  // for memcpy()
#include "mm/vm.h" 
#include "mm/mm.h" 

#include "call/syscall.h" // for debugging w/ print_strace() calls

bool use_fuzzy_buff;

uintptr_t fuzzy_buff;
uintptr_t fuzzy_buff_end;
int fuzzy_buff_size;
int fuzzy_buff_count;

buff_entry* head;
buff_entry* tail;

bool fuzzy_buff_init() {
  print_strace("fuzzy_buff_init !!!!\n");
  /* initialize timing buffer memory */
  // size of page defined by RISCV_PAGE_SIZE in vm_defs.h
  uintptr_t starting_vpn = vpn(EYRIE_ANON_REGION_START);
  int pte_flags = PTE_R | PTE_W | PTE_D | PTE_A;
  uintptr_t valid_pages;
  int req_pages = 1;
  while ((starting_vpn + req_pages) <= EYRIE_ANON_REGION_END) {
    valid_pages = test_va_range(starting_vpn, req_pages);

    if (req_pages <= valid_pages) {
      uintptr_t alloc_result = alloc_page(starting_vpn, pte_flags, false);
      // if alloc_page fails
      if (alloc_result == 0) {
        use_fuzzy_buff = false;
        return false;
      }
      fuzzy_buff = alloc_result;
      break;
    }
    else
      starting_vpn += valid_pages + 1;
  }

  fuzzy_buff_size = RISCV_PAGE_SIZE;
  fuzzy_buff_end = fuzzy_buff + fuzzy_buff_size;
  fuzzy_buff_count = 0;
  head = NULL;
  tail = NULL;

  use_fuzzy_buff = true;
  return true;
}

bool fuzzy_buff_push(void* dest, void* data, size_t data_size) {
  // TODO(chungmcl):
  // - If you run out of space, just ask SM to wait until 
  // next interval, then dequeue the thing (note that
  // this makes it so that a buffer of even size zero
  // should still work with timing stuff!)
  size_t add_size = sizeof(buff_entry) + data_size;

  buff_entry* entry_ptr;

  if (fuzzy_buff_count == 0) {
    entry_ptr = (buff_entry*)fuzzy_buff;
    head = entry_ptr;
    tail = entry_ptr;
  } else {
    uintptr_t head_raw = (uintptr_t)head;
    uintptr_t next_raw = (uintptr_t)tail + sizeof(buff_entry) + tail->data_size;
    if (tail > head) {
      if ((size_t)(fuzzy_buff_end - next_raw) >= add_size) {
        //        head <------- [USED SPACE] -------> tail <---- [FREE SPACE] ---->
        // start ~~^                                                         end ~~^
        entry_ptr = (buff_entry*)next_raw;
      } else if ((size_t)(head_raw - fuzzy_buff) >= add_size) {
        //          <------- [FREE SPACE] -------> head <------ [USED SPACE] ------> tail 
        // start ~~^                                                               end ~~^
        entry_ptr = (buff_entry*)fuzzy_buff;
      } else {
        return false;
      }
    } else {
      if ((size_t)(head_raw - next_raw) >= add_size) {
        //          |---- [USED SPACE] ----> tail <---- [FREE SPACE] ----> head <---- [USED SPACE] ----|
        // start ~~^                                                                              end ~~^
        entry_ptr = (buff_entry*)next_raw;
      } else {
        return false;
      }
    }
  }
  
  unsigned long time = sbi_get_time();
  entry_ptr->next = head;
  entry_ptr->data_size = data_size;
  entry_ptr->write_time = time + 2 * sbi_get_interval_len();
  // print_strace("write time: %d\n", entry_ptr->write_time);

  entry_ptr->dest = dest;
  memcpy(entry_ptr->data_copy, data, data_size);

  fuzzy_buff_count += 1;
  tail->next = entry_ptr;
  tail = entry_ptr;
  if (fuzzy_buff_count == 0) {
    head = tail;
  }

  return true;
}

int fuzzy_buff_flush_due_items(unsigned long curr_time) {
  buff_entry* curr = head;
  int count = 0;
  int fuzzy_buff_count_copy = fuzzy_buff_count;
  for (int i = 0; i < fuzzy_buff_count_copy; i++) {
    if (curr_time >= curr->write_time) {
      // print_strace("fuzzy_buff_flush_due_items() time: %lu\n", curr_time);
      // print_strace("fuzzy_buff_flush_due_items() write_time: %lu\n", curr->write_time);
      if (fuzzy_buff_remove()) {
        count += 1;
      } else return -1;
      // print_strace("\n");
    } else {
      // print_strace("curr time: %d && write time: %d\n", curr_time, curr->write_time);
      return count;
    }
    curr = curr->next;
  }
  return count;
}

int fuzzy_buff_flush() {
  int count = 0;
  while (fuzzy_buff_count != 0) {
    int items_flushed = fuzzy_buff_flush_due_items(sbi_pause());
    if (items_flushed != -1) {
      count += items_flushed;
    } else return -1;
  }
  return count;
}

bool fuzzy_buff_remove() {
  if (fuzzy_buff_count > 0) {
    memcpy(head->dest, head->data_copy, head->data_size);
    head = head->next;
    fuzzy_buff_count -= 1;
    return true;
  } else return false;
}

int fuzzy_buff_get_count() {
  return fuzzy_buff_count;
}

void fuzzy_buff_ipi_handle(struct sbi_scratch *scratch) {
  int flushed_items = fuzzy_buff_flush_due_items(sbi_get_time());
  
  if (flushed_items < 0) {
    print_strace("!!! fuzzy_buff_flush() failed !!!\n");
  }

  // signal that our interrupt is done
  csr_clear(sip, SIE_SSIE);
}

// for print_strace() to work, make sure to change the line:
//  set(eyrie_plugins "freemem")
// to:
//  set(eyrie_plugins "freemem strace_debug")
// in the CMakeLists.txt of whatever app you're
// building
// void debug_fuzzy_buff() {
//   buff_entry* curr = head;
//   print_strace("-------------------------------------------------------------------\n");
//   print_strace("fuzzy_buff: %p\n", (void*)fuzzy_buff);
//   print_strace("Head: %p\n", (void*)head);
//   print_strace("Tail: %p\n\n", (void*)tail);
//   for (int i = 0; i < fuzzy_buff_count; i++) {
//     print_strace("Element #: %i\n", i);
//     print_strace("curr->next: %p\n", (void*)curr->next);
//     print_strace("curr->dest: %p\n", (void*)curr->dest);
//     print_strace("curr->write_time: %lu\n", curr->write_time);
//     print_strace("curr->data_size: %zu\n", curr->data_size);
//     print_strace("curr->data_copy: %lu\n", *curr->data_copy);
//     print_strace("\n");
//     curr = curr->next;
//   }
// }