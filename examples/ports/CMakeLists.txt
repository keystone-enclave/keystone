set(eapp_send_bin sender)
set(eapp_recv_bin receiver)
set(eapp_send_src eapp/sender.c)
set(eapp_recv_src eapp/receiver.c)
set(host_bin ports-runner)
set(host_src host/host.cpp)
set(runner_script run.sh)
set(package_name "ports.ke")
set(package_script "./run.sh")

if(RISCV32)
  set(eyrie_plugins "freemem io_syscall linux_syscall env_setup callee rv32")
else()
  set(eyrie_plugins "freemem io_syscall linux_syscall env_setup callee")
endif()

# eapp

add_executable(${eapp_send_bin} ${eapp_send_src})
target_link_libraries(${eapp_send_bin} "-static" ${KEYSTONE_LIB_EAPP} pthread)

add_executable(${eapp_recv_bin} ${eapp_recv_src})
target_link_libraries(${eapp_recv_bin} "-static" ${KEYSTONE_LIB_EAPP} pthread)

# host

add_executable(${host_bin} ${host_src})
target_link_libraries(${host_bin} pthread ${KEYSTONE_LIB_HOST} ${KEYSTONE_LIB_EDGE})

# runner script
add_custom_target(ports-runner-script
        BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/${runner_script}
        DEPENDS ${runner_script}
        COMMAND rsync ${CMAKE_CURRENT_SOURCE_DIR}/${runner_script}
                  ${CMAKE_CURRENT_BINARY_DIR}/${runner_script} )

# add target for Eyrie runtime (see keystone.cmake)

set(eyrie_files_to_copy .options_log eyrie-rt)
add_eyrie_runtime(ports-eyrie
  ${eyrie_plugins}
  ${eyrie_files_to_copy})

# add target for packaging (see keystone.cmake)

add_keystone_package(ports-package
  ${package_name}
  ${package_script}
  ${eyrie_files_to_copy} ${eapp_send_bin} ${eapp_recv_bin} ${host_bin} ${runner_script})

add_dependencies(ports-package ports-eyrie ports-runner-script)

# add package to the top-level target
add_dependencies(examples ports-package)
