set(eapp_bin myhello)
set(eapp_src eapp/myhello.c)
set(host_bin myhello-runner)
set(host_src host/myhello_host.cpp)
set(package_name "myhello.ke")
set(package_script "./myhello-runner myhello eyrie-rt loader.bin")
set(eyrie_plugins "io_syscall linux_syscall env_setup")

# Enclave Application (EApp)
add_executable(${eapp_bin} ${eapp_src})
target_link_libraries(${eapp_bin} "-static")

# Host Application
add_executable(${host_bin} ${host_src})
target_link_libraries(${host_bin} ${KEYSTONE_LIB_HOST} ${KEYSTONE_LIB_EDGE})

# Eyrie Runtime
set(eyrie_files_to_copy .options_log eyrie-rt loader.bin)
add_eyrie_runtime(${eapp_bin}-eyrie
  ${eyrie_plugins}
  ${eyrie_files_to_copy})

# Packaging into a .ke archive
add_keystone_package(${eapp_bin}-package
  ${package_name}
  ${package_script}
  ${eyrie_files_to_copy} ${eapp_bin} ${host_bin})

add_dependencies(${eapp_bin}-package ${eapp_bin}-eyrie)
add_dependencies(examples ${eapp_bin}-package)