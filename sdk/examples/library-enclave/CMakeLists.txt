set(host_bin library-runner)
set(host_src host/host.cpp)
set(package_name "library-enclave.ke")
set(package_script "./library-runner libc.so.6")

# host

add_executable(${host_bin} ${host_src})
target_link_libraries(${host_bin} ${KEYSTONE_LIB_HOST} ${KEYSTONE_LIB_EDGE})

add_keystone_package(${eapp_bin}-package
  ${package_name}
  ${package_script}
  ${host_bin}
  ${CMAKE_CURRENT_SOURCE_DIR}/libc.so.6)

# add package to the top-level target
add_dependencies(examples ${eapp_bin}-package)
