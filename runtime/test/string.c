#include "../util/string.c"

#include "mock.h"

static void
test_unaligned_memcpy(void** ctx) {
  char src[32];
  char dst[32] = {};
  char acc     = 1;
  // populate acc
  for (int i = 0; i < 32; i++) {
    acc *= 251;
    src[i] = acc;
  }
  // do the memcpy
  memcpy(dst + 1, src + 3, 29);
  // check
  assert_int_equal(dst[0], 0);
  assert_int_equal(dst[30], 0);
  assert_int_equal(dst[31], 0);
  for (int i = 0; i < 29; i++) {
    assert_int_equal(dst[i + 1], src[i + 3]);
  }
}

static void
test_unaligned_memset(void** ctx) {
  char dst[32] = {};
  memset(dst + 1, 'A', 29);
  assert_int_equal(dst[0], 0);
  assert_int_equal(dst[30], 0);
  assert_int_equal(dst[31], 0);
  for (int i = 0; i < 29; i++) {
    assert_int_equal(dst[i + 1], 'A');
  }
}

int
main() {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_unaligned_memcpy),
      cmocka_unit_test(test_unaligned_memset),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
