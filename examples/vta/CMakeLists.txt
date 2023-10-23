
####################
## Configuration ###
####################

set(model resnet18_v2)

if(NOT BUILDROOT_HOST_DIR)
    message(FATAL_ERROR BUILDROOT_HOST_DIR undefined)
endif()

if(NOT BUILDROOT_TARGET_DIR)
    message(FATAL_ERROR BUILDROOT_TARGET_DIR undefined)
endif()


if(RISCV32)
    set(eyrie_plugins "freemem io_syscall linux_syscall env_setup drivers callee rv32")
else()
    set(eyrie_plugins "freemem io_syscall linux_syscall env_setup drivers callee")
endif()

####################
## Model sources ###
####################

## Various directories ##
set(vta_src_dir     ${CMAKE_CURRENT_SOURCE_DIR})
set(gen_dir         ${CMAKE_CURRENT_BINARY_DIR}/gen)
set(bins_dir        ${gen_dir}/bins/)
set(lib_dir         ${gen_dir}/lib/${model})
set(pad_dir         ${gen_dir}/pad/${model})
set(src_dir         ${gen_dir}/src/${model})

# Raw binary files from TVM
set(lib_bin         ${bins_dir}/${model}/graphlib.tar)
set(graph_bin       ${bins_dir}/${model}/graph.json)
set(params_bin      ${bins_dir}/${model}/params.bin)

# Padded binary files
set(graph_bin_pad   ${pad_dir}/graph.json.pad)
set(params_bin_pad  ${pad_dir}/params.bin.pad)

# Source files
set(graph_src       ${src_dir}/graph.json.c)
set(params_src      ${src_dir}/params.bin.c)
set(imgs_src        ${gen_dir}/imgs.xz.c)

# Aligned source files
set(graph_src_pad   ${src_dir}/graph.json.pad.c)
set(params_src_pad  ${src_dir}/params.bin.pad.c)

## Helper macros ##

macro(pad input output to)
    add_custom_command(OUTPUT ${output} DEPENDS ${ARGN}
        BYPRODUCTS ${output}
        COMMAND mkdir -p ${pad_dir}
        COMMAND cp ${input} ${output}
        COMMAND truncate -s $$\(\(
            ( $$\(stat --printf="%s" ${output} \)
                / ${to} + 1) * ${to}\)\) ${output})
endmacro(pad)

macro(hexify input output name)
    add_custom_command(OUTPUT ${output} DEPENDS ${bins_dir}
        BYPRODUCTS ${output}
        COMMAND mkdir -p ${src_dir}
        COMMAND xxd -i -n ${name} ${input} > ${output})
endmacro(hexify)

macro(hexify_aligned input output name to)
    add_custom_command(OUTPUT ${output} DEPENDS ${input}
        BYPRODUCTS ${output}
        COMMAND mkdir -p ${src_dir}
        COMMAND  echo \"__attribute__\(\(aligned\(${to}\)\)\)\" > ${output} &&
            xxd -i -n ${name} ${input} >> ${output})
endmacro(hexify_aligned)

## Binaries ##

add_custom_command(OUTPUT ${bins_dir}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/bins/bins.tar.xz
        COMMAND mkdir -p ${gen_dir}
        COMMAND tar xvf ${CMAKE_CURRENT_SOURCE_DIR}/bins/bins.tar.xz -C ${gen_dir}
        COMMAND touch ${bins_dir}) # Make sure target is not continuously out of date

## Operator library ##

# The operators are typically linked directly into downstream consumers, so don't need to be padded/hexified
add_custom_command(OUTPUT ${lib_dir}/devc.o
        BYPRODUCTS ${lib_dir}/lib0.o ${lib_dir}/lib1.o
        DEPENDS ${bins_dir}
        COMMAND mkdir -p ${lib_dir}
        COMMAND tar -xf ${lib_bin} -C ${lib_dir}
        COMMAND touch ${lib_dir})

## Graph ##
pad(${graph_bin} ${graph_bin_pad} 0x1000 ${bins_dir})
hexify_aligned(${graph_bin_pad} ${graph_src_pad} graph_json_pad 0x1000)
hexify(${graph_bin} ${graph_src} graph_json)

## Params ##
pad(${params_bin} ${params_bin_pad} 0x1000 ${bins_dir})
hexify_aligned(${params_bin_pad} ${params_src_pad} params_bin_pad 0x1000)
hexify(${params_bin} ${params_src} params_bin 0x1000)

## Images ##
hexify(${CMAKE_CURRENT_SOURCE_DIR}/bins/imgs.xz ${imgs_src} imgs_xz)

## Final targets ##
add_library(${model} STATIC ${graph_src_pad} ${graph_src} ${params_src_pad} ${params_src} ${imgs_src})
target_compile_options(${model} PRIVATE -ffunction-sections -fdata-sections)

add_library(${model}-ops STATIC ${lib_dir}/lib0.o ${lib_dir}/lib1.o ${lib_dir}/devc.o)
target_compile_options(${model}-ops PRIVATE -ffunction-sections -fdata-sections)
set_target_properties(${model}-ops PROPERTIES LINKER_LANGUAGE C)

##############
## Enclaves ##
##############

# This macro handles the common parts for every VTA enclave. The names of any eapp targets
# are passed in ARGN

macro(add_vta_example name)
    # Generate host functionality, including runner script
    add_executable(vta-${name}-runner ${vta_src_dir}/host/host.cpp)
    target_link_libraries(vta-${name}-runner ${KEYSTONE_LIB_HOST} ${KEYSTONE_LIB_EDGE})
    set(vta-${name}-host-bins ${CMAKE_CURRENT_BINARY_DIR}/vta-${name}-runner)

    add_custom_target(vta-${name}-runner-script
            BYPRODUCTS ${gen_dir}/run-${name}.sh
            DEPENDS ${vta_src_dir}/scripts/run-${name}.sh
            COMMAND mkdir -p ${gen_dir}
            COMMAND cp ${vta_src_dir}/scripts/run-${name}.sh ${gen_dir}/run-${name}.sh)

    foreach(dep IN ITEMS ${ARGN})
        list(APPEND vta-${name}-eapp-bins ${CMAKE_CURRENT_BINARY_DIR}/${dep})
    endforeach(dep)

    add_eyrie_runtime(vta-${name}-eyrie
            ${eyrie_plugins}
            .options_log eyrie-rt)

    add_keystone_package(vta-${name}-package
            "vta-${name}.ke"
            "./run-${name}.sh"
            # Eyrie files
            .options_log eyrie-rt
            # Host runners
            ${vta-${name}-host-bins} ${gen_dir}/run-${name}.sh
            # Eapp bins
            ${vta-${name}-eapp-bins})

    add_dependencies(vta-${name}-package vta-${name}-eyrie vta-${name}-runner-script)
    add_dependencies(examples vta-${name}-package)
endmacro()

macro(vta_default_dirs target)
    target_include_directories(${target} PRIVATE ${vta_src_dir}/include ${BUILDROOT_HOST_DIR}/usr/include)
    target_link_directories(${target} PRIVATE ${BUILDROOT_HOST_DIR}/usr/lib/tvm ${BUILDROOT_TARGET_DIR}/usr/lib)
#    target_link_options(${target} PRIVATE -Wl,--gc-sections)
endmacro(vta_default_dirs)

## Shared functionality library ##

add_library(vta-shared STATIC ${CMAKE_CURRENT_SOURCE_DIR}/eapp/shared/platform.cc ${CMAKE_CURRENT_SOURCE_DIR}/eapp/shared/images.cc)
vta_default_dirs(vta-shared)
target_compile_options(vta-shared PRIVATE
        -ffunction-sections -fdata-sections -mcmodel=medany)

# Various examples
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/eapp/direct)