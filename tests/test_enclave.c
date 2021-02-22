#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../src/enclave.c"

static void test_is_create_args_valid()
{
  struct keystone_sbi_create args;

  // should return true
  args.epm_region.paddr = 0x4000;
  args.epm_region.size = 0x2000;
  args.utm_region.paddr = 0x8000;
  args.utm_region.size = 0x2000;
  args.runtime_paddr = 0x4000;
  args.user_paddr = 0x5000;
  args.free_paddr = 0x6000;
  assert_int_equal(is_create_args_valid(&args), 1);

  // true even if epm and utm overlap
  // overlapping will be prevented by the pmp_region_init_atomic
  args.utm_region.paddr = 0x3000;
  assert_int_equal(is_create_args_valid(&args), 1);
  args.utm_region.paddr = 0x6000;

  // false if runtime is not within the epm
  args.runtime_paddr = 0x3000;
  assert_int_equal(is_create_args_valid(&args), 0);
  args.runtime_paddr = 0x4000;

  // false if user is not within the epm
  args.user_paddr = 0x7000;
  assert_int_equal(is_create_args_valid(&args), 0);
  args.user_paddr = 0x5000;

  // false if free is not within the epm
  args.free_paddr = 0x7000;
  assert_int_equal(is_create_args_valid(&args), 0);
  args.free_paddr = 0x6000;

  // true even if runtime size == 0
  args.user_paddr = 0x4000;
  assert_int_equal(is_create_args_valid(&args), 1);
  args.user_paddr = 0x5000;

  // true even if user size == 0
  args.free_paddr = 0x5000;
  assert_int_equal(is_create_args_valid(&args), 1);
  args.free_paddr = 0x6000;

  // false if runtime > user
  args.runtime_paddr = 0x5f00;
  assert_int_equal(is_create_args_valid(&args), 0);
  args.runtime_paddr  = 0x4000;

  // false if user > free
  args.free_paddr = 0x4500;
  assert_int_equal(is_create_args_valid(&args), 0);
  args.free_paddr = 0x6000;

  // false if epm overflows
  args.epm_region.size = -1UL;
  assert_int_equal(is_create_args_valid(&args), 0);
  args.epm_region.size = 0x2000;
}

static void test_context_switch_to_enclave()
{

}

static void test_get_enclave_region_after_init()
{
  enclave_id eid;
  int memid;
  enum enclave_region_type type;

  enclave_init_metadata();

  for (eid=0; eid < ENCL_MAX; eid++)
  {
    // testing get_enclave_region_index
    assert_int_equal( get_enclave_region_index(eid, REGION_EPM), -1 );
    assert_int_equal( get_enclave_region_index(eid, REGION_UTM), -1 );
    assert_int_equal( get_enclave_region_index(eid, REGION_OTHER), -1 );
    assert_int_equal( get_enclave_region_index(eid, REGION_INVALID), 0 );

    // testing get_enclave_region_size
    assert_int_equal( get_enclave_region_size(eid, REGION_EPM), 0 );
    assert_int_equal( get_enclave_region_size(eid, REGION_UTM), 0 );
    assert_int_equal( get_enclave_region_size(eid, REGION_OTHER), 0 );
    assert_int_equal( get_enclave_region_size(eid, REGION_INVALID), 0 );

    // testing get_enclave_region_base
    assert_int_equal( get_enclave_region_base(eid, REGION_EPM), 0 );
    assert_int_equal( get_enclave_region_base(eid, REGION_UTM), 0 );
    assert_int_equal( get_enclave_region_base(eid, REGION_OTHER), 0 );
    assert_int_equal( get_enclave_region_base(eid, REGION_INVALID), 0 );

  }
}

static void test_get_enclave_region_index()
{
  enclave_id eid;
  int memid;
  enum enclave_region_type type;

  enclave_init_metadata();

  enclaves[0].regions[2].type = REGION_OTHER;
  enclaves[0].regions[2].pmp_rid = 4;

  assert_int_equal( get_enclave_region_index(0, REGION_OTHER), 2 );
}

int main()
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_is_create_args_valid),
    cmocka_unit_test(test_context_switch_to_enclave),
    cmocka_unit_test(test_get_enclave_region_after_init),
    cmocka_unit_test(test_get_enclave_region_index),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
