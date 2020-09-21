#pragma once

#if defined(USE_FREEMEM) && defined(USE_PAGING)

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef union merkle_node {
  struct {
    uintptr_t ptr;
    uint8_t hash[32];
    union {
      struct {
        union merkle_node *left, *right;
      };
      union merkle_node* children[2];
    };
  };
  struct {
    uint64_t raw_words[8];
  };
} merkle_node_t;

int
merk_insert(merkle_node_t* root, uintptr_t key, const uint8_t hash[32]);
bool
merk_verify(
    volatile merkle_node_t* root, uintptr_t key, const uint8_t hash_out[32]);

#endif
