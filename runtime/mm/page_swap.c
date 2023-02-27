#include "mm/page_swap.h"

#if defined(USE_FREEMEM) && defined(USE_PAGING)

#include <stdatomic.h>
#include <stddef.h>

#include "crypto/aes.h"
#include "crypto/merkle.h"
#include "mm/paging.h"
#include "call/sbi.h"
#include "crypto/sha256.h"
#include "mm/vm_defs.h"

#define NUM_CTR_INDIRECTS 24
static uintptr_t ctr_indirect_ptrs[NUM_CTR_INDIRECTS];

static uintptr_t paging_next_backing_page_offset;
static uintptr_t paging_inc_backing_page_offset_by;

uintptr_t
paging_alloc_backing_page() {
  uintptr_t offs_update =
      (paging_next_backing_page_offset + paging_inc_backing_page_offset_by) %
      paging_backing_region_size();

  /* no backing page available */
  if (offs_update == 0) {
    // cycled through all the pages
    warn("no backing page avaiable");
    return 0;
  }

  uintptr_t next_page =
      paging_backing_region() + paging_next_backing_page_offset;
  assert(IS_ALIGNED(next_page, RISCV_PAGE_BITS));

  paging_next_backing_page_offset = offs_update;
  return next_page;
}

unsigned int
paging_remaining_pages() {
  return (paging_backing_region_size() - paging_next_backing_page_offset) /
         RISCV_PAGE_SIZE;
}

static uintptr_t
gcd(uintptr_t a, uintptr_t b) {
  while (b) {
    uintptr_t tmp = b;
    b             = a % b;
    a             = tmp;
  }
  return a;
}

static uintptr_t
find_coprime_of(uintptr_t n) {
  uintptr_t res;
  do {
    res = n / 2 + sbi_random() % (n / 2);
  } while (gcd(res, n) != 1);
  return res;
}

void
pswap_init(void) {
  uintptr_t backing_pages = paging_backing_region_size() / RISCV_PAGE_SIZE;
  uintptr_t inc           = find_coprime_of(backing_pages);

  paging_inc_backing_page_offset_by = inc * RISCV_PAGE_SIZE;
  warn("num_pages = %zx, pagesize_inc = %zx", backing_pages, inc);

  paging_next_backing_page_offset = 0;
}

static uint64_t*
pswap_pageout_ctr(uintptr_t page) {
  assert(paging_backpage_inbounds(page));
  size_t idx          = (page - paging_backing_region()) >> RISCV_PAGE_BITS;
  size_t indirect_idx = idx / (RISCV_PAGE_SIZE / 8);
  size_t interior_idx = idx % (RISCV_PAGE_SIZE / 8);
  assert(indirect_idx < NUM_CTR_INDIRECTS);

  if (!ctr_indirect_ptrs[indirect_idx]) {
    ctr_indirect_ptrs[indirect_idx] = paging_alloc_backing_page();
    // Fill ptr pages with random values so our counters start unpredictable
    rt_util_getrandom((void*)ctr_indirect_ptrs[indirect_idx], RISCV_PAGE_SIZE);
  }

  return (uint64_t*)(ctr_indirect_ptrs[indirect_idx]) + interior_idx;
}

#ifdef USE_PAGE_CRYPTO
static volatile atomic_bool pswap_boot_key_reserved = false;
static volatile atomic_bool pswap_boot_key_set      = false;
static uint8_t pswap_boot_key[32];

static void
pswap_establish_boot_key(void) {
  uint8_t boot_key_tmp[32];

  if (atomic_load(&pswap_boot_key_set)) {
    // Key already set
    return;
  }

  rt_util_getrandom(boot_key_tmp, 32);

  if (atomic_flag_test_and_set(&pswap_boot_key_reserved)) {
    // Lost the race; key already being set. Spin until finished.
    while (!atomic_load(&pswap_boot_key_set))
      ;
    return;
  }

  memcpy(pswap_boot_key, boot_key_tmp, 32);
  atomic_store(&pswap_boot_key_set, true);
}
#endif  // USE_PAGE_CRYPTO

#ifdef USE_PAGE_HASH
static merkle_node_t paging_merk_root = {};
#endif

static void
pswap_encrypt(const void* addr, void* dst, uint64_t pageout_ctr) {
  size_t len = RISCV_PAGE_SIZE;

#ifdef USE_PAGE_CRYPTO
  pswap_establish_boot_key();
  uint8_t iv[32] = {0};
  WORD key_sched[80];
  aes_key_setup(pswap_boot_key, key_sched, 256);

  memcpy(iv + 8, &pageout_ctr, 8);

  aes_encrypt_ctr((uint8_t*)addr, len, (uint8_t*)dst, key_sched, 256, iv);
#else
  memcpy(dst, addr, len);
#endif
}

static void
pswap_decrypt(const void* addr, void* dst, uint64_t pageout_ctr) {
  size_t len = RISCV_PAGE_SIZE;

#ifdef USE_PAGE_CRYPTO
  pswap_establish_boot_key();
  uint8_t iv[32] = {0};
  WORD key_sched[80];
  aes_key_setup(pswap_boot_key, key_sched, 256);

  memcpy(iv + 8, &pageout_ctr, 8);

  aes_decrypt_ctr((uint8_t*)addr, len, (uint8_t*)dst, key_sched, 256, iv);
#else
  memcpy(dst, addr, len);
#endif
}

static void
pswap_hash(uint8_t* hash, void* page_addr, uint64_t pageout_ctr) {
#ifdef USE_PAGE_HASH
  SHA256_CTX hasher;

  sha256_init(&hasher);
  sha256_update(&hasher, page_addr, RISCV_PAGE_SIZE);
  sha256_update(&hasher, (uint8_t*)&pageout_ctr, sizeof(pageout_ctr));
  sha256_final(&hasher, hash);
#endif
}

/* evict a page from EPM and store it to the backing storage
 * back_page (PA1) <-- epm_page (PA2) <-- swap_page (PA1)
 * if swap_page is 0, no need to write epm_page
 */
void
page_swap_epm(uintptr_t back_page, uintptr_t epm_page, uintptr_t swap_page) {
  assert(paging_epm_inbounds(epm_page));
  assert(paging_backpage_inbounds(back_page));

  char buffer[RISCV_PAGE_SIZE] = {};
  if (swap_page) {
    assert(swap_page == back_page);
    memcpy(buffer, (void*)swap_page, RISCV_PAGE_SIZE);
  }

  uint64_t* pageout_ctr    = pswap_pageout_ctr(back_page);
  uint64_t old_pageout_ctr = *pageout_ctr;
  uint64_t new_pageout_ctr = old_pageout_ctr + 1;

  uint8_t new_hash[32];
  pswap_hash(new_hash, (void*)epm_page, new_pageout_ctr);
  pswap_encrypt((void*)epm_page, (void*)back_page, new_pageout_ctr);

  if (swap_page) {
    uint8_t old_hash[32];
    pswap_decrypt((void*)buffer, (void*)epm_page, old_pageout_ctr);
    pswap_hash(old_hash, (void*)epm_page, old_pageout_ctr);

#ifdef USE_PAGE_HASH
    bool ok = merk_verify(&paging_merk_root, back_page, old_hash);
    assert(ok);
#endif
  }

#ifdef USE_PAGE_HASH
  merk_insert(&paging_merk_root, back_page, new_hash);
#endif

  *pageout_ctr = new_pageout_ctr;

  return;
}

#endif
