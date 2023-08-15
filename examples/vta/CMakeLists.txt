set(eapp_bin vta-enclave)
set(eapp_src eapp/vta.cc)
set(host_bin vta-runner)
set(host_src host/host.cpp)
set(runner_script run.sh)
set(package_name "vta.ke")
set(package_script "./run.sh")
set(model resnet18_v2)

if(RISCV32)
    set(eyrie_plugins "freemem io_syscall linux_syscall env_setup drivers rv32")
else()
    set(eyrie_plugins "freemem io_syscall linux_syscall env_setup drivers")
endif()

# eapp

# For now, just fake the model generation. Ideally we'd have this run at build time, but buildroot's
# LLVM doesn't seem too happy with TVM.

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/graphlib.tar ${CMAKE_CURRENT_BINARY_DIR}/graph.json ${CMAKE_CURRENT_BINARY_DIR}/params.bin ${CMAKE_CURRENT_BINARY_DIR}/model.stamp
#                    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_model.py ${CMAKE_CURRENT_BINARY_DIR}
                    COMMAND touch ${CMAKE_CURRENT_BINARY_DIR}/model.stamp)

# Hexify into C files
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/bins/${model}/graph.json
		${CMAKE_CURRENT_BINARY_DIR}/bins/${model}/params.bin
		${CMAKE_CURRENT_BINARY_DIR}/bins/${model}/graphlib.tar
		${CMAKE_CURRENT_BINARY_DIR}/bins/cat.bin
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/bins.tar.xz
	COMMAND tar xvf ${CMAKE_CURRENT_SOURCE_DIR}/bins.tar.xz -C ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/graph.json.c
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/bins/${model}/graph.json
	COMMAND xxd -i -n graph_json ${CMAKE_CURRENT_BINARY_DIR}/bins/${model}/graph.json > ${CMAKE_CURRENT_BINARY_DIR}/graph.json.c)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/params.bin.c
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/bins/${model}/params.bin
	COMMAND xxd -i -n params_bin ${CMAKE_CURRENT_BINARY_DIR}/bins/${model}/params.bin > ${CMAKE_CURRENT_BINARY_DIR}/params.bin.c)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/devc.o ${CMAKE_CURRENT_BINARY_DIR}/lib1.o ${CMAKE_CURRENT_BINARY_DIR}/lib0.o
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/bins/${model}/graphlib.tar
        COMMAND tar -xf ${CMAKE_CURRENT_BINARY_DIR}/bins/${model}/graphlib.tar -C ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/cat.bin.c
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/bins/cat.bin
        COMMAND xxd -i -n cat_bin  ${CMAKE_CURRENT_BINARY_DIR}/bins/cat.bin > ${CMAKE_CURRENT_BINARY_DIR}/cat.bin.c)


add_executable(${eapp_bin} ${eapp_src} ${CMAKE_CURRENT_BINARY_DIR}/graph.json.c ${CMAKE_CURRENT_BINARY_DIR}/params.bin.c  ${CMAKE_CURRENT_BINARY_DIR}/cat.bin.c ${CMAKE_CURRENT_BINARY_DIR}/devc.o  ${CMAKE_CURRENT_BINARY_DIR}/lib1.o  ${CMAKE_CURRENT_BINARY_DIR}/lib0.o)
target_link_libraries(${eapp_bin} "-static" memory graph_executor common vta ${KEYSTONE_LIB_EAPP} stdc++ m)
target_include_directories(${eapp_bin} PRIVATE eapp)

# host

add_executable(${host_bin} ${host_src})
target_link_libraries(${host_bin} ${KEYSTONE_LIB_HOST} ${KEYSTONE_LIB_EDGE})

# runner script
add_custom_target(vta-runner-script
        BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/${runner_script}
        DEPENDS scripts/${runner_script}
        COMMAND rsync ${CMAKE_CURRENT_SOURCE_DIR}/scripts/${runner_script}
        ${CMAKE_CURRENT_BINARY_DIR}/${runner_script} )

# add target for Eyrie runtime (see keystone.cmake)

set(eyrie_files_to_copy .options_log eyrie-rt)
add_eyrie_runtime(${eapp_bin}-eyrie
        ${eyrie_plugins}
        ${eyrie_files_to_copy})

# add target for packaging (see keystone.cmake)

add_keystone_package(${eapp_bin}-package
        ${package_name}
        ${package_script}
        ${eyrie_files_to_copy} ${eapp_bin} ${host_bin} ${runner_script})

add_dependencies(${eapp_bin}-package ${eapp_bin}-eyrie vta-runner-script)

# add package to the top-level target
add_dependencies(examples ${eapp_bin}-package)
