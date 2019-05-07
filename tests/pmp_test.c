#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../pmp.c"

static void test_search_rightmost_unset()
{
  // static int search_rightmost_unset(uint32 bitmap, int max, uint32_t mask)

  // test with mask = 0x1
  assert_int_equal(search_rightmost_unset(0x0, 8, 0x1), 0);
  assert_int_equal(search_rightmost_unset(0x2, 8, 0x1), 0);
  assert_int_equal(search_rightmost_unset(0x1, 8, 0x1), 1);
  assert_int_equal(search_rightmost_unset(0xffef, 8, 0x1), 4);
  assert_int_equal(search_rightmost_unset(0xffff, 8, 0x1), -1);

  assert_int_equal(search_rightmost_unset(0xff, 8, 0x1), -1);
  assert_int_equal(search_rightmost_unset(0xff, 16, 0x1), 8);
  assert_int_equal(search_rightmost_unset(0xffff, 16, 0x1), -1);
  assert_int_equal(search_rightmost_unset(0xffff, 31, 0x1), 16);
  assert_int_equal(search_rightmost_unset(0xffffffff, 31, 0x1), -1);

  // test with mask = 0x3
  assert_int_equal(search_rightmost_unset(0x0, 8, 0x3), 0);
  assert_int_equal(search_rightmost_unset(0x2, 8, 0x3), 2);
  assert_int_equal(search_rightmost_unset(0x1, 8, 0x3), 1);
  assert_int_equal(search_rightmost_unset(0xfcef, 31, 0x3), 8);
  assert_int_equal(search_rightmost_unset(0xffef, 8, 0x3), -1);
  assert_int_equal(search_rightmost_unset(0xffff, 8, 0x3), -1);

  // test with invalid arguments
  expect_assert_failure(search_rightmost_unset(0x0, 36, 0x1));
  expect_assert_failure(search_rightmost_unset(0x0, 16, 0x5));
}

static void test_get_free_region_idx()
{
  region_def_bitmap = 0x20;
  assert_int_equal(get_free_region_idx(), 0);
  region_def_bitmap = 0x3f;
  assert_int_equal(get_free_region_idx(), 6);

  // tear down
  region_def_bitmap = 0x0;
}

static void test_get_free_reg_idx()
{
  reg_bitmap = 0x20;
  assert_int_equal(get_free_reg_idx(), 0);
  reg_bitmap = 0x3f;
  assert_int_equal(get_free_reg_idx(), 6);

  // tear down
  reg_bitmap = 0x0;
}

static void test_get_conseq_free_reg_idx()
{
  reg_bitmap = 0x1;
  assert_int_equal(get_conseq_free_reg_idx(), 0x1);
  reg_bitmap = 0x2e;
  assert_int_equal(get_conseq_free_reg_idx(), 6);
  reg_bitmap = 0xffffffff;
  assert_int_equal(get_conseq_free_reg_idx(), -1);

  // tear down
  reg_bitmap = 0x0;
}

int main()
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_search_rightmost_unset),
    cmocka_unit_test(test_get_free_region_idx),
    cmocka_unit_test(test_get_free_reg_idx),
    cmocka_unit_test(test_get_conseq_free_reg_idx),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
