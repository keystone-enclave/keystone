# set(eapp_bin management-core-test)
# set(eapp_src eapp/eapp_fuzzy_time_test.c)
set(host_bin management-core-test-runner)
set(host_src host/host_management_core_test.c)
set(package_name "management-core-test.ke")
# run management-core-test on CPU 1 (which we set up to be management core)
# set(package_script "taskset 0x1 ./management-core-test-runner")
set(package_script "./management-core-test-runner")

if(RISCV32)
  set(eyrie_plugins "rv32 freemem")
else()
  set(eyrie_plugins "freemem strace_debug")
endif()

# eapp
# add_executable(${eapp_bin} ${eapp_src})
# target_link_libraries(${eapp_bin} "-nostdlib -static" ${KEYSTONE_LIB_EAPP} ${KEYSTONE_LIB_EDGE})
# 
# target_include_directories(${eapp_bin}
#   PUBLIC ${KEYSTONE_SDK_DIR}/include/app
#   PUBLIC ${KEYSTONE_SDK_DIR}/include/edge)

# host

add_executable(${host_bin} ${host_src})
# chungmcl: added -lpthread
target_link_libraries(${host_bin} "-lpthread" ${KEYSTONE_LIB_HOST} ${KEYSTONE_LIB_EDGE})
# add -std=c++11 flag
set_target_properties(${host_bin}
  PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO
)
target_include_directories(${host_bin}
  PUBLIC ${KEYSTONE_SDK_DIR}/include/host
  PUBLIC ${KEYSTONE_SDK_DIR}/include/edge)

# add target for packaging (see keystone.cmake)

add_keystone_package(${host_bin}-package
  ${package_name}
  ${package_script} ${host_bin})

# add package to the top-level target
add_dependencies(examples ${host_bin}-package)
