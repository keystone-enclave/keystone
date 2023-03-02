set(eapp_caller_bin caller)
set(eapp_callee_bin callee)
set(eapp_caller_src eapp/caller.c)
set(eapp_callee_src eapp/callee.c)
set(host_bin maps-runner)
set(host_src host/host.cpp)
set(runner_script run.sh)
set(package_name "maps.ke")
set(package_script "./run.sh")

if(RISCV32)
    set(eyrie_plugins "freemem io_syscall linux_syscall env_setup callee rv32")
else()
    set(eyrie_plugins "freemem io_syscall linux_syscall env_setup callee")
endif()

# eapp

add_exportable_function(test1 0x4000 eapp/export/test1.c)

add_executable(${eapp_caller_bin} ${eapp_caller_src})
target_link_libraries(${eapp_caller_bin} "-static" ${KEYSTONE_LIB_EAPP} pthread test1)

#add_dependencies(${eapp_caller_bin} test1_bin)

add_executable(${eapp_callee_bin} ${eapp_callee_src})
target_link_libraries(${eapp_callee_bin} "-static" ${KEYSTONE_LIB_EAPP} pthread)

# host

add_executable(${host_bin} ${host_src})
target_link_libraries(${host_bin} pthread ${KEYSTONE_LIB_HOST} ${KEYSTONE_LIB_EDGE})

# runner script
add_custom_target(maps-runner-script
        BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/${runner_script}
        DEPENDS ${runner_script}
        COMMAND rsync ${CMAKE_CURRENT_SOURCE_DIR}/${runner_script}
        ${CMAKE_CURRENT_BINARY_DIR}/${runner_script} )

# add target for Eyrie runtime (see keystone.cmake)

set(eyrie_files_to_copy .options_log eyrie-rt)
add_eyrie_runtime(maps-eyrie
        ${eyrie_plugins}
        ${eyrie_files_to_copy})

# add target for packaging (see keystone.cmake)

add_keystone_package(maps-package
        ${package_name}
        ${package_script}
        ${eyrie_files_to_copy} ${eapp_caller_bin} ${eapp_callee_bin} ${host_bin} ${runner_script})

add_dependencies(maps-package maps-eyrie maps-runner-script)

# add package to the top-level target
add_dependencies(examples maps-package)
