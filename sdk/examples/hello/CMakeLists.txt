set(eapp_bin hello)
set(eapp_src eapp/hello.c)
set(host_bin hello-runner)
set(host_src host/host.cpp)
set(package_name "hello.ke")
set(package_script "./hello-runner hello eyrie-rt")

if(RISCV32)
  set(eyrie_plugins "freemem io_syscall linux_syscall env_setup rv32")
else()
  set(eyrie_plugins "freemem io_syscall linux_syscall env_setup")
endif()

# eapp

add_executable(${eapp_bin} ${eapp_src})
target_link_libraries(${eapp_bin} "-static")

# host

add_executable(${host_bin} ${host_src})
target_link_libraries(${host_bin} ${KEYSTONE_LIB_HOST} ${KEYSTONE_LIB_EDGE})

# add target for Eyrie runtime (see keystone.cmake)

set(eyrie_files_to_copy .options_log eyrie-rt)
add_eyrie_runtime(${eapp_bin}-eyrie
  ${eyrie_plugins}
  ${eyrie_files_to_copy})

# add target for packaging (see keystone.cmake)

add_keystone_package(${eapp_bin}-package
  ${package_name}
  ${package_script}
  ${eyrie_files_to_copy} ${eapp_bin} ${host_bin})

add_dependencies(${eapp_bin}-package ${eapp_bin}-eyrie)

# add package to the top-level target
add_dependencies(examples ${eapp_bin}-package)
