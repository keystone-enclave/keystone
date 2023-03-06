#if defined(USE_PAGE_HASH)

#include "crypto/merkle.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

#include "mm/paging.h"
#include "crypto/sha256.h"
#include "mm/vm_defs.h"

#ifndef MERK_SILENT
#define MERK_LOG printf
#else
#define MERK_LOG(...)
#endif

_Static_assert(sizeof(merkle_node_t) == 64, "merkle_node_t is not 64 bytes!");

#define MERK_NODES_PER_PAGE (RISCV_PAGE_SIZE / sizeof(merkle_node_t))

typedef struct merkle_page_freelist {
  uint64_t free[MERK_NODES_PER_PAGE / 64];
  uint16_t free_count;
  bool in_freelist;
  struct merkle_page_freelist* next;
} merkle_page_freelist_t;

_Static_assert(
    sizeof(merkle_page_freelist_t) <= sizeof(merkle_node_t),
    "merkle_page_freelist_t does not fit in one merkle_node_t!");

static merkle_page_freelist_t*
merk_alloc_page(void) {
  void* page                        = (void*)paging_alloc_backing_page();
  merkle_page_freelist_t* free_list = (merkle_page_freelist_t*)page;
  memset(free_list, 0, sizeof(*free_list));

  for (size_t i = 0; i < MERK_NODES_PER_PAGE; i += 64) {
    size_t this_page_nodes = MERK_NODES_PER_PAGE - i;
    free_list->free[i / 64] =
        (this_page_nodes < 64) * (1ull << this_page_nodes) - 1;
  }
  free_list->free[0] &= ~(uint64_t)1;
  free_list->free_count = MERK_NODES_PER_PAGE - 1;

  return free_list;
}

static merkle_page_freelist_t* merk_free_list = NULL;

static merkle_node_t*
merk_reserve_node_in_page(merkle_page_freelist_t* free_list) {
  if (!free_list->free_count) return NULL;

  for (size_t i = 0; i < MERK_NODES_PER_PAGE / 64; i++) {
    if (free_list->free[i]) {
      size_t free_idx = __builtin_ctzll(free_list->free[i]);
      free_list->free[i] &= ~(1ull << free_idx);
      free_list->free_count--;

      merkle_node_t* page = (merkle_node_t*)free_list;
      assert(free_idx != 0);

      return page + free_idx;
    }
  }
  return NULL;
}

static merkle_node_t*
merk_alloc_node(void) {
  while (merk_free_list && merk_free_list->free_count == 0) {
    // Clear out the unfree lists
    merk_free_list->in_freelist = false;
    merk_free_list              = merk_free_list->next;
  }

  if (!merk_free_list) {
    merk_free_list              = merk_alloc_page();
    merk_free_list->in_freelist = true;
  }

  merkle_node_t* out = merk_reserve_node_in_page(merk_free_list);
  return out;
}

static void
merk_free_node(merkle_node_t* node) {
  uintptr_t page                    = (uintptr_t)node & ~(RISCV_PAGE_SIZE - 1);
  merkle_page_freelist_t* free_list = (merkle_page_freelist_t*)page;
  size_t idx                        = node - (merkle_node_t*)page;

  assert(idx < MERK_NODES_PER_PAGE);
  assert((free_list->free[idx / 64] & (1ull << (idx % 64))) == 0);

  free_list->free[idx / 64] |= (1ull << (idx % 64));
  free_list->free_count++;

  if (!free_list->in_freelist) {
    free_list->next        = merk_free_list;
    merk_free_list         = free_list;
    free_list->in_freelist = true;
  }
}

static bool
merk_verify_single_node(
    const merkle_node_t* node, const merkle_node_t* left,
    const merkle_node_t* right) {
  SHA256_CTX hasher;
  uint8_t calculated_hash[32];

  sha256_init(&hasher);

  if (left) {
    sha256_update(&hasher, (uint8_t*)&left->ptr, sizeof right->ptr);
    sha256_update(&hasher, left->hash, 32);
  }
  if (right) {
    sha256_update(&hasher, (uint8_t*)&right->ptr, sizeof right->ptr);
    sha256_update(&hasher, right->hash, 32);
  }

  if (!left && !right) {
    return true;
  }

  sha256_final(&hasher, calculated_hash);
  return memcmp(calculated_hash, node->hash, 32) == 0;
}

static void
merk_hash_single_node(
    merkle_node_t* node, const merkle_node_t* left,
    const merkle_node_t* right) {
  SHA256_CTX hasher;
  sha256_init(&hasher);
  if (left) {
    sha256_update(&hasher, (uint8_t*)&left->ptr, sizeof left->ptr);
    sha256_update(&hasher, left->hash, 32);
  }
  if (right) {
    sha256_update(&hasher, (uint8_t*)&right->ptr, sizeof right->ptr);
    sha256_update(&hasher, right->hash, 32);
  }
  sha256_final(&hasher, node->hash);
}

bool
merk_verify(
    volatile merkle_node_t* root, uintptr_t key, const uint8_t hash[32]) {
  merkle_node_t node = *root;
  if (!root->right) return false;

  merkle_node_t left;
  merkle_node_t right = *root->right;

  // Verify root node
  if (!merk_verify_single_node(&node, NULL, &right)) {
    MERK_LOG("Error verifying root!\n");
    return false;
  }

  node = right;

  for (int i = 0;; i++) {
    // node is a leaf, so return its hash check
    if (!node.left && !node.right) {
      return memcmp(hash, node.hash, 32) == 0;
    }

    // Load in the next layer. This is to prevent race conditions
    if (node.left) left = *(volatile merkle_node_t*)node.left;
    if (node.right) right = *(volatile merkle_node_t*)node.right;

    bool node_ok = merk_verify_single_node(
        &node, node.left ? &left : NULL, node.right ? &right : NULL);
    if (!node_ok) {
      MERK_LOG("Error at node with ptr %zx in layer %d\n", node.ptr, i);
      return false;
    }

    // BST traversal
    if (key < node.ptr) {
      node = left;
    } else {
      node = right;
    }
  }
}

// Insert a node at the leaf position. May insert a new intermediate node or
// overwrite an existing one. Returns the node modified.
static merkle_node_t*
merk_splice_node(merkle_node_t* leaf, merkle_node_t* node) {
  if (node->ptr == leaf->ptr) {
    // We've specified a key that already exists, so overwrite the old node.
    merk_free_node(leaf);
    return node;
  }

  merkle_node_t* new_parent = merk_alloc_node();

  if (node->ptr < leaf->ptr) {
    *new_parent = (merkle_node_t){
        .ptr   = leaf->ptr,
        .left  = node,
        .right = leaf,
    };
    merk_hash_single_node(new_parent, node, leaf);
  } else {
    *new_parent = (merkle_node_t){
        .ptr   = node->ptr,
        .left  = leaf,
        .right = node,
    };
    merk_hash_single_node(new_parent, leaf, node);
  }

  return new_parent;
}

#define MERK_MAX_DEPTH 32
static merkle_node_t* intermediate_nodes[MERK_MAX_DEPTH] = {};

int
merk_insert(merkle_node_t* root, uintptr_t key, const uint8_t hash[32]) {
  merkle_node_t new_node_data = {
      .ptr = key,
  };
  memcpy(new_node_data.hash, hash, 32);

  merkle_node_t* new_node            = merk_alloc_node();
  *(volatile merkle_node_t*)new_node = new_node_data;

  // The root never contains data, only a single pointer to the start
  // of data on its right side.
  // This is to better ensure a total split between the root and other
  // nodes, as the root is merely a "guardian" which must reside in secure
  // memory while others don't need to.
  if (!root->right) {
    merk_hash_single_node(root, NULL, &new_node_data);
    root->right = new_node;
    return 0;
  }

  intermediate_nodes[0] = root;
  int i;

  for (i = 0; i < MERK_MAX_DEPTH - 1; i++) {
    // Walk down the BST to find an appropriate location to store our new node.

    merkle_node_t* parent = intermediate_nodes[i];

    // Traverse the BST

    bool traverse_left        = key < parent->ptr;
    bool child_idx            = traverse_left ^ 1;
    intermediate_nodes[i + 1] = parent->children[child_idx];
    if (!intermediate_nodes[i + 1]) break;
  }

  if (i == MERK_MAX_DEPTH) {
    printf(
        "Inserted to merkle tree with problematic key insertion order! "
        "This has led to an unbalanced tree exceeding the depth capacity of "
        "%d. "
        "Aborting!",
        MERK_MAX_DEPTH);
    assert(false);
  }

  merkle_node_t curr_node = *intermediate_nodes[i];

  for (; i > 0; i--) {
    // Here we walk back up the tree to percolate up our new hashes.
    // We keep the previous iteration's merkle node, as well as the current
    // iteration's merkle node, in secure memory to avoid any race conditions
    // after writing to DRAM in the last step.
    //
    // We otherwise aren't concerned that an attacker will modify any data
    // in the tree during this stage, as doing so will compromise the integrity
    // of the tree such that the user will be alerted upon attempting any
    // accesses to the compromised location.
    //
    // We also mark accesses to parent_ptr as volatile to ensure they get
    // written and read with the correct access pattern.

    merkle_node_t* parent_ptr = intermediate_nodes[i - 1];
    merkle_node_t* node_ptr   = intermediate_nodes[i];
    merkle_node_t parent      = *(volatile merkle_node_t*)parent_ptr;
    merkle_node_t sibling;
    bool has_sibling = parent.left && parent.right;
    int node_idx     = parent.right == node_ptr;

    if (has_sibling)
      sibling = *(volatile merkle_node_t*)parent.children[!node_idx];

    // Check to see that the sibling we pull is valid.
    // We don't care about node_ptr races here, because if it's been tampered
    // with, verifying against parent will fail (eventually). And we already
    // stored the node data we'll be using later on in curr_node, anyway.
    if (has_sibling) {
      const merkle_node_t* copied_children[2];
      copied_children[node_idx]  = node_ptr;
      copied_children[!node_idx] = &sibling;
      if (!merk_verify_single_node(
              &parent, copied_children[0], copied_children[1]))
        return -1;
    }

    // At the leaf, insert the new node. merk_splice_node will handle updating
    // the hashes, so if we insert a new intermediate node we can treat that as
    // the new "leaf" / bottom-layer node.
    if (!curr_node.left && !curr_node.right) {
      node_ptr = intermediate_nodes[i] = merk_splice_node(node_ptr, new_node);
      curr_node                        = *node_ptr;

      parent.children[node_idx] = node_ptr;
    }

    // Hash our data from the saved curr_node and sibling.
    assert(node_ptr == parent.children[node_idx]);
    const merkle_node_t* copied_children[2];
    copied_children[node_idx]  = &curr_node;
    copied_children[!node_idx] = has_sibling ? &sibling : NULL;
    merk_hash_single_node(&parent, copied_children[0], copied_children[1]);

    // Writeback the previous changed node now
    *(volatile merkle_node_t*)node_ptr = curr_node;
    curr_node                          = parent;
  }

  // Writeback the final changed node
  *(volatile merkle_node_t*)root = curr_node;

  return 0;
}

#endif
