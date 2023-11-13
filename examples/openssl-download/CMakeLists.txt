### START ADD_BINS
set(eapp_downloader_bin downloader)
### END ADD_BINS

### START ADD_SRCS
set(eapp_downloader_src eapp/downloader.c)
### END ADD_SRCS

set(host_bin openssl-download-runner)
set(host_src host/downloader-host.cpp)
set(runner_script run.sh)
set(package_name "openssl-download.ke")
set(package_script "./run.sh")

if(RISCV32)
    set(eyrie_plugins "freemem io_syscall net_syscall linux_syscall env_setup drivers callee rv32")
else()
    set(eyrie_plugins "freemem io_syscall net_syscall linux_syscall env_setup drivers callee")
endif()

# eapp

### START ADD_EXPORTABLE_FUNCTIONS
### END ADD_EXPORTABLE_FUNCTIONS

include_directories(AFTER ${KEYSTONE_SDK_DIR}/include)

# set(certificate_file /etc/ssl/certs/UCA_Extended_Validation_Root.pem)
set(certificate_file /etc/ssl/certs/USERTrust_RSA_Certification_Authority.pem) # works specifically for homes.cs.washington.edu
# set(certificate_file /etc/ssl/certs/ca-certificates.crt) # all default certificates on your host Linux system
add_custom_target(certificate.c
                        DEPENDS ${certificate_file}
                        BYPRODUCTS certificate.c
                        COMMAND cat ${certificate_file} | xxd -i -n certificate_bytes > certificate.c
)
### START ADD_EXECUTABLES
add_executable(${host_bin} ${host_src})
target_link_libraries(${host_bin} "-static -Wl,-v" pthread ${KEYSTONE_LIB_HOST} ${KEYSTONE_LIB_EDGE})

add_executable(${eapp_downloader_bin} ${eapp_downloader_src} certificate.c ${BUILDROOT_HOST_DIR}/riscv64-buildroot-linux-gnu/sysroot/lib/crt1.o)
target_link_libraries(${eapp_downloader_bin} "-static -nostdlib -Wl,-v -Wl,--verbose -v --verbose" ${KEYSTONE_LIB_EAPP} -lssl -lcrypto -lc_musl -lgcc_musl -lm_musl -lutil_musl
                        ### START LINK_EXPORT_FUNCTIONS downloader
                        ### END LINK_EXPORT_FUNCTIONS downloader
)
### END ADD_EXECUTABLES

# runner script
add_custom_target(openssl-download-runner-script
        BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/${runner_script}
        DEPENDS ${runner_script}
        COMMAND rsync ${CMAKE_CURRENT_SOURCE_DIR}/${runner_script}
        ${CMAKE_CURRENT_BINARY_DIR}/${runner_script} )
# add target for Eyrie runtime (see keystone.cmake)

set(eyrie_files_to_copy .options_log eyrie-rt)
add_eyrie_runtime(openssl-download-eyrie
        ${eyrie_plugins}
        ${eyrie_files_to_copy})

# add target for packaging (see keystone.cmake)

add_keystone_package(openssl-download-package
        ${package_name}
        ${package_script}
        ${eyrie_files_to_copy}
### START ADD_KEYSTONE_PACKAGE_BINS
        ${eapp_downloader_bin}
### END ADD_KEYSTONE_PACKAGE_BINS
        ${host_bin}
        ${runner_script}
)

add_dependencies(openssl-download-package openssl-download-eyrie openssl-download-runner-script)

# add package to the top-level target
add_dependencies(examples openssl-download-package)
