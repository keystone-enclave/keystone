#define _GNU_SOURCE

#include "crypto/merkle.h"

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/mman.h>

#define MERK_SILENT
#include "../crypto/merkle.c"
#include "mock.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

void
sbi_exit_enclave(uintptr_t code) {
  exit(code);
}

uintptr_t
paging_alloc_backing_page() {
  void* out = mmap(
      NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  assert_int_not_equal(out, MAP_FAILED);
  return (uintptr_t)out;
}

#define RAND_REGION_ENTRIES 1000
#define RAND_ENTRY_SIZE 64

const uint8_t*
random_region() {
  static uint8_t* random_region_buf = NULL;
  if (!random_region_buf) {
    random_region_buf = (uint8_t*)malloc(RAND_REGION_ENTRIES * RAND_ENTRY_SIZE);
    for (size_t i = 0; i < RAND_REGION_ENTRIES * RAND_ENTRY_SIZE; i++) {
      random_region_buf[i] = (uint8_t)rand();
    }
  }
  return random_region_buf;
}

size_t*
shuffled_idxs(size_t max) {
  size_t* shuffled_idxs = (size_t*)malloc(sizeof(size_t) * max);
  for (size_t i = 0; i < max; i++) shuffled_idxs[i] = i;

  for (size_t i = max - 1; i > 0; i--) {
    size_t j         = rand() % i + 1;
    size_t tmp       = shuffled_idxs[i];
    shuffled_idxs[i] = shuffled_idxs[j];
    shuffled_idxs[j] = tmp;
  }
  return shuffled_idxs;
}

static merkle_node_t
random_region_insert(merkle_node_t* root) {
  const uint8_t* region = random_region();
  size_t* idxs          = shuffled_idxs(RAND_REGION_ENTRIES);

  SHA256_CTX sha;

  for (int i = 0; i < RAND_REGION_ENTRIES; i++) {
    const uint8_t* subregion = region + idxs[i] * RAND_ENTRY_SIZE;
    uint8_t hash[32];

    sha256_init(&sha);
    sha256_update(&sha, subregion, RAND_ENTRY_SIZE);
    sha256_final(&sha, hash);

    int res = merk_insert(root, (uintptr_t)subregion, hash);
    assert_int_equal(res, 0);
  }

  free(idxs);
}

static merkle_node_t
random_region_tree() {
  merkle_node_t root = {};
  random_region_insert(&root);
  return root;
}

static size_t
count_verify_fails(merkle_node_t* tree) {
  size_t total_verify_fails = 0;
  SHA256_CTX sha;

  size_t* idxs = shuffled_idxs(RAND_REGION_ENTRIES);

  for (size_t ri = 0; ri < RAND_REGION_ENTRIES; ri++) {
    const uint8_t* region = random_region() + idxs[ri] * RAND_ENTRY_SIZE;
    uint8_t region_hash[32];
    sha256_init(&sha);
    sha256_update(&sha, region, RAND_ENTRY_SIZE);
    sha256_final(&sha, region_hash);
    total_verify_fails += !merk_verify(tree, (uintptr_t)region, region_hash);
  }

  free(idxs);
  return total_verify_fails;
}

struct merk_stats_s {
  size_t max_depth, min_depth;
  size_t elems, leaves;
  double avg_depth;
};

struct merk_stats_s
merk_stats(const merkle_node_t* root) {
  const merkle_node_t *left = root->left, *right = root->right;

  struct merk_stats_s out = {
      .max_depth = 1,
      .min_depth = 1,
      .elems     = 1,
      .leaves    = 0,
      .avg_depth = 1,
  };

  if (!left && !right) {
    out.leaves = 1;
    return out;
  }

  struct merk_stats_s lstats, rstats;
  if (left) {
    lstats        = merk_stats(left);
    out.max_depth = lstats.max_depth;
    out.min_depth = lstats.min_depth;
    out.elems += lstats.elems;
    out.leaves += lstats.leaves;
    out.avg_depth = lstats.avg_depth + 1;
  }
  if (right) {
    rstats        = merk_stats(right);
    out.max_depth = rstats.max_depth;
    out.min_depth = rstats.min_depth;
    out.elems += rstats.elems;
    out.leaves += rstats.leaves;
    out.avg_depth = rstats.avg_depth + 1;
  }
  if (left && right) {
    double both_elems = lstats.elems + rstats.elems;
    double lweight    = lstats.elems / both_elems;
    double rweight    = rstats.elems / both_elems;
    out.max_depth     = MAX(lstats.max_depth, rstats.max_depth) + 1;
    out.min_depth     = MIN(lstats.max_depth, rstats.max_depth) + 1;
    out.avg_depth = lweight * lstats.avg_depth + rweight * rstats.avg_depth + 1;
  }
  return out;
}

static void
test_verify_nonexistant() {
  merkle_node_t root = {};
  uint8_t zeros[32]  = {};
  assert_false(merk_verify(&root, 1, zeros));
}

static void
test_insert_and_verify_1() {
  merkle_node_t root       = {};
  const uint8_t* rand_hash = random_region();

  int res = merk_insert(&root, 1, rand_hash);
  assert_int_equal(res, 0);
  assert_true(merk_verify(&root, 1, rand_hash));
}

static void
test_insert_and_verify_2() {
  merkle_node_t root         = {};
  const uint8_t* rand_hash_1 = random_region();
  const uint8_t* rand_hash_2 = random_region() + 32;

  int res = merk_insert(&root, 1, rand_hash_1);
  assert_int_equal(res, 0);
  res = merk_insert(&root, 2, rand_hash_2);
  assert_int_equal(res, 0);
  assert_true(merk_verify(&root, 1, rand_hash_1));
  assert_true(merk_verify(&root, 2, rand_hash_2));
}

static void
test_insert_and_verify_many() {
  merkle_node_t root = random_region_tree();
  assert_int_equal(count_verify_fails(&root), 0);
  struct merk_stats_s stats_0 = merk_stats(&root);

  random_region_insert(&root);
  assert_int_equal(count_verify_fails(&root), 0);
  struct merk_stats_s stats_1 = merk_stats(&root);

  assert_memory_equal(&stats_0, &stats_1, sizeof(struct merk_stats_s));
}

static void
test_random_insert_stats() {
  merkle_node_t root        = random_region_tree();
  struct merk_stats_s stats = merk_stats(&root);
  assert_int_equal(stats.leaves, RAND_REGION_ENTRIES);
  assert_in_range(stats.elems, stats.leaves, stats.leaves * 2);
  assert_true(stats.min_depth > log2(RAND_REGION_ENTRIES));
  assert_true(stats.max_depth < 4 * log2(RAND_REGION_ENTRIES));
  assert_true(stats.avg_depth < 2 * log2(RAND_REGION_ENTRIES));
}

static void
test_poison_data() {
  merkle_node_t root        = random_region_tree();
  size_t poison_idx         = rand() % RAND_REGION_ENTRIES;
  const uint8_t* poison_ptr = random_region() + poison_idx * RAND_ENTRY_SIZE;

  SHA256_CTX sha;
  uint8_t hash[32];
  sha256_init(&sha);
  sha256_update(&sha, poison_ptr, RAND_ENTRY_SIZE);
  sha256_final(&sha, hash);

  // Flip a random bit in the hash to simulate a tampered entry
  hash[rand() & 31] ^= 1 << (rand() & 7);

  bool res = merk_verify(&root, (uintptr_t)poison_ptr, hash);
  assert_false(res);
}

static void
flip_random_bit(uint8_t* buf, size_t size) {
  buf[rand() % size] ^= 1 << (rand() & 7);
}

static void
test_poison_leaf() {
  merkle_node_t root  = random_region_tree();
  merkle_node_t* node = &root;

  // Randomly walk the tree until we get to a leaf
  while (node->left || node->right) {
    merkle_node_t* next[2];
    int num_next   = 0;
    next[num_next] = node->left;
    num_next += !!node->left;
    next[num_next] = node->right;
    num_next += !!node->right;

    int taken = rand() % num_next;
    node      = next[taken];
  }

  uintptr_t key = node->ptr;
  uint8_t* hash = node->hash;
  // Simulate a tampered entry
  flip_random_bit(hash, 32);

  bool res = merk_verify(&root, key, hash);
  assert_false(res);
}

static void
test_poison_root() {
  merkle_node_t root = random_region_tree();
  flip_random_bit(root.hash, 32);

  size_t total_verify_fails = count_verify_fails(&root);
  assert_int_equal(total_verify_fails, RAND_REGION_ENTRIES);
}

static void
test_insert_corrupt_insert() {
  merkle_node_t root = random_region_tree();

  // Find a node that has a leaf on the right, and a sibling or nephew on the
  // left
  // TODO: not all trees may have this structure
  merkle_node_t* node = &root;
  assert_non_null(node->right);
  assert_non_null(node->right->right);

  while (node->right->right) {
    node = node->right;
  }

  merkle_node_t* leaf = node->right;
  // Find the position of the sibling/nephew leaf
  merkle_node_t* sibling = node->left;

  assert_non_null(leaf);
  assert_non_null(sibling);

  while (sibling->left) {
    sibling = sibling->left;
  }

  assert_null(leaf->left);
  assert_null(leaf->right);
  assert_null(sibling->left);
  assert_null(sibling->right);

  // Check to make sure both start off okay
  bool ok = merk_verify(&root, leaf->ptr, leaf->hash);
  ok &= merk_verify(&root, sibling->ptr, sibling->hash);
  assert_true(ok);

  // When we corrupt the leaf hash, we expect the leaf check to fail
  flip_random_bit(leaf->hash, 32);

  merkle_node_t leaf_copy = *leaf, sibling_copy = *sibling;
  ok = merk_verify(&root, leaf_copy.ptr, leaf_copy.hash);
  assert_false(ok);

  // Test that merk_insert doesn't incorrectly "validate" a hash that isn't the
  // one we inserted
  int res = merk_insert(&root, sibling_copy.ptr, sibling_copy.hash);
  assert_int_not_equal(res, 0);
  ok = merk_verify(&root, leaf_copy.ptr, leaf_copy.hash);
  assert_false(ok);
}

static void
test_corrupt_key() {
  merkle_node_t root = {};
  SHA256_CTX sha;

  int res = merk_insert(&root, 1, random_region());
  assert_int_equal(res, 0);
  res = merk_insert(&root, 2, random_region() + 32);
  assert_int_equal(res, 0);

  assert_true(merk_verify(&root, 1, random_region()));
  assert_true(merk_verify(&root, 2, random_region() + 32));

  // Swap the keys for entries 1 and 2
  assert_non_null(root.right);
  merkle_node_t *first = root.right->left, *second = root.right->right;
  assert_non_null(first);
  assert_non_null(second);

  assert_int_equal(first->ptr, 1);
  first->ptr = 2;
  assert_int_equal(second->ptr, 2);
  second->ptr = 1;

  assert_false(merk_verify(&root, 1, random_region()));
  assert_false(merk_verify(&root, 2, random_region() + 32));
}

int
main() {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_verify_nonexistant),
      cmocka_unit_test(test_insert_and_verify_1),
      cmocka_unit_test(test_insert_and_verify_2),
      cmocka_unit_test(test_insert_and_verify_many),
      cmocka_unit_test(test_random_insert_stats),
      cmocka_unit_test(test_poison_data),
      cmocka_unit_test(test_poison_leaf),
      cmocka_unit_test(test_poison_root),
      cmocka_unit_test(test_insert_corrupt_insert),
      cmocka_unit_test(test_corrupt_key),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}