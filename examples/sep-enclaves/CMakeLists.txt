# Nom des binaires
set(eapp1_bin password_check)
set(eapp2_bin multiplier)
set(host_bin multi-enclave-runner)
set(host_src host/multi_enclave_host.cpp)

# Chemin du package et nom
set(package_name "multi.enclaves.ke")
set(package_script "./multi-enclave-runner password_check multiplier eyrie-rt loader.bin")
set(eyrie_plugins "io_syscall linux_syscall env_setup")

# Dépendances EAPP
set(eapp_common_includes
  PUBLIC ${KEYSTONE_SDK_DIR}/include/app
  PUBLIC ${KEYSTONE_SDK_DIR}/include/edge
)

# Enclave 1 : password_check
add_executable(${eapp1_bin} eapp_password_check/password_check.c)
target_link_libraries(${eapp1_bin} "-nostdlib -static" ${KEYSTONE_LIB_EAPP} ${KEYSTONE_LIB_EDGE})
target_include_directories(${eapp1_bin} ${eapp_common_includes})

# Enclave 2 : multiplier
add_executable(${eapp2_bin} eapp_multiplier/multiplier.c)
target_link_libraries(${eapp2_bin} "-nostdlib -static" ${KEYSTONE_LIB_EAPP} ${KEYSTONE_LIB_EDGE})
target_include_directories(${eapp2_bin} ${eapp_common_includes})

# Host commun
add_executable(${host_bin} ${host_src})
target_link_libraries(${host_bin} ${KEYSTONE_LIB_HOST} ${KEYSTONE_LIB_EDGE})
set_target_properties(${host_bin}
  PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO
)
target_include_directories(${host_bin}
  PUBLIC ${KEYSTONE_SDK_DIR}/include/host
  PUBLIC ${KEYSTONE_SDK_DIR}/include/edge)

# Eyrie runtime
set(eyrie_files_to_copy .options_log eyrie-rt loader.bin)
add_eyrie_runtime(multi-enclave-eyrie
  ${eyrie_plugins}
  ${eyrie_files_to_copy})

# Packaging
add_keystone_package(multi-enclave-package
  ${package_name}
  ${package_script}
  ${eyrie_files_to_copy} ${eapp1_bin} ${eapp2_bin} ${host_bin})

add_dependencies(multi-enclave-package multi-enclave-eyrie)

# Ajouter au build global
add_dependencies(examples multi-enclave-package)
