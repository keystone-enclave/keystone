set(host_bin test-runner)
set(host_src test-runner.cpp edge_wrapper.cpp)

if(RISCV32)
   set(eyrie_plugins "freemem rv32")
else()
   set(eyrie_plugins "freemem")
endif()

set(package_name "tests.ke")
set(package_script "./run-test.sh")

###############################################
#                Test Apps                    #
###############################################
# to add a test, (1) add the test to the following variable
set(all_test_bins
  stack
  loop
  malloc
  long-nop
  fibonacci
  fib-bench
  attestation
  untrusted
  data-sealing)

# and (2) define the recipe of the test below:

# stack
add_executable(stack stack/stack.S)
target_link_libraries(stack ${KEYSTONE_LIB_EAPP})

# loop
add_executable(loop loop/loop.s)
target_link_libraries(loop ${KEYSTONE_LIB_EAPP})

# malloc
add_executable(malloc malloc/malloc.c)
target_link_libraries(malloc ${KEYSTONE_LIB_EAPP})

# long-nop
add_executable(long-nop long-nop/long-nop.S ${CMAKE_CURRENT_BINARY_DIR}/add_long.S)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/add_long.S
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/long-nop/generate_func.sh 4097
  ${CMAKE_CURRENT_BINARY_DIR}/add_long.S
  ${CMAKE_CURRENT_SOURCE_DIR}/long-nop/nop.s
  ${CMAKE_CURRENT_SOURCE_DIR}/long-nop/func_base.s
  ${CMAKE_CURRENT_SOURCE_DIR}/long-nop/nop.h
)
target_link_libraries(long-nop ${KEYSTONE_LIB_EAPP})

# fibonacci
add_executable(fibonacci fibonacci/fibonacci.c)
target_link_libraries(fibonacci ${KEYSTONE_LIB_EAPP})

# fib-bench
add_executable(fib-bench fib-bench/fib-bench.c)
target_link_libraries(fib-bench ${KEYSTONE_LIB_EAPP})

# attestation
add_executable(attestation attestation/attestation.c attestation/edge_wrapper.c)
target_link_libraries(attestation ${KEYSTONE_LIB_EAPP} ${KEYSTONE_LIB_EDGE})

# untrusted
add_executable(untrusted untrusted/untrusted.c untrusted/edge_wrapper.c)
target_link_libraries(untrusted ${KEYSTONE_LIB_EAPP} ${KEYSTONE_LIB_EDGE})

# data-sealing
add_executable(data-sealing data-sealing/data-sealing.c)
target_link_libraries(data-sealing ${KEYSTONE_LIB_EAPP} ${KEYSTONE_LIB_EDGE})

###############################################
# a script for running all test enclaves
set(test_script_tmp ${CMAKE_CURRENT_BINARY_DIR}/tmp/run-test.sh)
set(test_script ${CMAKE_CURRENT_BINARY_DIR}/run-test.sh)

file(WRITE ${test_script_tmp} "")
foreach (test IN ITEMS ${all_test_bins})
  file(APPEND ${test_script_tmp} "echo 'testing ${test}'\n")
  if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${test}/retval)
    execute_process(COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/${test}/retval OUTPUT_VARIABLE retval)
    file(APPEND ${test_script_tmp} "./${host_bin} ${test} eyrie-rt --retval ${retval}")
  else()
    file(APPEND ${test_script_tmp} "./${host_bin} ${test} eyrie-rt\n")
  endif()
endforeach(test)

file(COPY ${test_script_tmp} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
  FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)
file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/tmp)

# linker flags for all tests
set_target_properties(${all_test_bins}
  PROPERTIES LINK_FLAGS "-nostdlib -static -T ${CMAKE_CURRENT_SOURCE_DIR}/app.lds")
###############################################

# host

add_executable(${host_bin} ${host_src})
target_link_libraries(${host_bin} ${KEYSTONE_LIB_HOST} ${KEYSTONE_LIB_EDGE} ${KEYSTONE_LIB_VERIFIER})

# add target for Eyrie runtime (see keystone.cmake)

set(eyrie_files_to_copy .options_log eyrie-rt)
add_eyrie_runtime(test-eyrie
  ${eyrie_plugins}
  ${eyrie_files_to_copy})

# add target for packaging (see keystone.cmake)

add_keystone_package(test-package
  ${package_name}
  ${package_script}
  ${test_script} ${eyrie_files_to_copy} ${all_test_bins} ${host_bin}
  )

add_dependencies(test-package test-eyrie)

# add package to the top-level target
add_dependencies(examples test-package)
