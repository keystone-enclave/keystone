cmake_minimum_required(VERSION 3.5)
project(keystone C)
include(ProcessorCount)
include(ExternalProject)
set(CMAKE_VERBOSE_MAKEFILE ON)

###############################################################################
## MACROS
###############################################################################

# add patch macro
macro(add_patch submodule patch working_directory patch_list)
  add_custom_command(OUTPUT ${patch}.applied
    WORKING_DIRECTORY ${working_directory}
    COMMAND patch --forward -p1 < ${patchdir}/${submodule}/${patch} || true
    COMMAND touch ${CMAKE_BINARY_DIR}/${patch}.applied
    COMMENT "Applying ${patch}")
  list(APPEND ${patch_list} ${patch}.applied)
endmacro()

macro(mkdir name dir)
  add_custom_command(OUTPUT ${dir}/.exists
    COMMAND mkdir -p ${dir}
    COMMAND touch ${dir}/.exists
  )
  set(${name} ${dir})
  set(${name}_exists ${dir}/.exists)
endmacro()


###############################################################################
## CONFIGURATION/VARIABLES
###############################################################################

set(USE_RUST_SM FALSE CACHE BOOL "Use Rust version of the security monitor.")
set(SM_CONFIGURE_ARGS --enable-opt=2 CACHE STRING "Security Monitor configure script arguments")
set(SM_PLATFORM "generic" CACHE STRING "Board name for SM hardware-specific functions")
set(platform ${SM_PLATFORM})
message(STATUS "platform=${platform}")

set(LINUX_SIFIVE FALSE CACHE BOOL "Build linux for sifive")
set(sifive ${LINUX_SIFIVE})
message(STATUS "sifive=${sifive}")

if((CMAKE_BUILD_TYPE MATCHES "Debug") OR (CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo"))
  message(STATUS "Using debug symbols")
  set(CFLAGS -g)
endif()

set(RISCV32 OFF CACHE BOOL "Build in 32-bit mode")
if(RISCV32)
  message(STATUS "riscv32")
  set(BITS 32)
  set(ABI ilp32d)
else()
  message(STATUS "riscv64")
  set(BITS 64)
  set(ABI lp64d)
endif()

set(KEYSTONE_SDK_DIR ${CMAKE_SOURCE_DIR}/sdk/build${BITS})

set(ISA rv${BITS}imafdc)
set(confdir ${CMAKE_SOURCE_DIR}/overlays/keystone/configs)
set(patchdir ${CMAKE_SOURCE_DIR}/overlays/keystone/patches)
set(cross_compile riscv${BITS}-buildroot-linux-gnu-)

set(sm_srcdir ${CMAKE_SOURCE_DIR}/sm)
mkdir(sm_wrkdir ${CMAKE_BINARY_DIR}/sm.build)
set(buildroot_srcdir ${CMAKE_SOURCE_DIR}/buildroot)
set(buildroot_wrkdir ${CMAKE_BINARY_DIR}/buildroot.build)
set(buildroot_config ${confdir}/qemu_riscv${BITS}_virt_defconfig)
set(overlay_dir ${CMAKE_BINARY_DIR}/overlay)
set(overlay_root ${overlay_dir}/root)
set(external_dir ${CMAKE_SOURCE_DIR}/overlays)
set(toolchain_dir ${buildroot_wrkdir}/per-package/host-gcc-final/host/bin)
set(bootrom_srcdir ${CMAKE_SOURCE_DIR}/bootrom)
set(bootrom_wrkdir ${buildroot_wrkdir}/images/)

if(firesim)
  message(STATUS "Firesim configs and patches")
  set(buildroot_config ${confdir}/riscv${BITS}_firesim_defconfig)
elseif(sifive)
  message(STATUS "SiFive Unleashed configs and patches")
  set(buildroot_config ${confdir}/riscv${BITS}_sifive_defconfig)
elseif(cva6)
  message(STATUS "CVA6 configs and patches")
  set(buildroot_config ${confdir}/riscv${BITS}_cva6_defconfig)
endif()

set(linux_image ${buildroot_wrkdir}/images/Image)
set(driver_srcdir ${CMAKE_SOURCE_DIR}/linux-keystone-driver)
set(fw_elf ${sm_wrkdir}/platform/${platform}/firmware/fw_jump.elf)
set(fw_bin ${sm_wrkdir}/platform/${platform}/firmware/fw_jump.bin)
set(initramfs_sysroot ${CMAKE_BINARY_DIR}/initramfs-sysroot)

###############################################################################
## COMPONENT: buildroot
###############################################################################

add_custom_command(OUTPUT ${overlay_dir} COMMAND mkdir -p ${overlay_dir})
add_custom_command(DEPENDS ${overlay_dir}
  OUTPUT ${overlay_root}
  ${overlay_root}/.ssh/id_rsa ${overlay_root}/.ssh/id_rsa.pub
  ${overlay_root}/.ssh/authorized_keys
  COMMAND mkdir -p ${overlay_root}
  COMMAND mkdir -p ${overlay_root}/.ssh
  COMMAND ssh-keygen -C 'root@keystone' -t rsa -f ${overlay_root}/.ssh/id_rsa -N ''
  COMMAND cp ${overlay_root}/.ssh/id_rsa.pub ${overlay_root}/.ssh/authorized_keys)

add_custom_command(OUTPUT ${buildroot_wrkdir} COMMAND mkdir -p ${buildroot_wrkdir})
add_custom_target("buildroot" ALL DEPENDS ${buildroot_srcdir} ${buildroot_wrkdir}/.config ${overlay_root} ${buildroot_wrkdir} "buildroot_toolchain"
  COMMAND $(MAKE) -s -C ${buildroot_srcdir} O=${buildroot_wrkdir} BR2_EXTERNAL=${external_dir}/keystone
            KEYSTONE_BOOTROM=${bootrom_srcdir} KEYSTONE_DRIVER=${driver_srcdir}
  COMMENT "Building buildroot"
)

add_custom_target("buildroot_toolchain" ALL DEPENDS ${buildroot_srcdir} ${buildroot_wrkdir}/.config
  COMMAND $(MAKE) -s -C ${buildroot_srcdir} O=${buildroot_wrkdir} BR2_EXTERNAL=${external_dir}/keystone
            KEYSTONE_BOOTROM=${bootrom_srcdir} KEYSTONE_DRIVER=${driver_srcdir} toolchain
  COMMENT "Building buildroot toolchain"
)

string(REPLACE "/" "\\/" overlay_dir_stripped ${overlay_dir})
add_custom_command(DEPENDS ${buildroot_config} OUTPUT ${buildroot_wrkdir}/.config
  COMMAND mkdir -p ${buildroot_wrkdir}
  COMMAND cp ${buildroot_config} ${buildroot_wrkdir}/.config
  COMMAND sed \"s/^BR2_ROOTFS_OVERLAY=.*/BR2_ROOTFS_OVERLAY=\\\"${overlay_dir_stripped}\\\"/g\" -i ${buildroot_wrkdir}/.config
  COMMAND $(MAKE) -s -C ${buildroot_srcdir} O=${buildroot_wrkdir} BR2_EXTERNAL=${external_dir}/keystone olddefconfig
  COMMENT "Configuring buildroot (overlay = ${overlay_dir})"
)

###############################################################################
## COMPONENT: security monitor (sm)
###############################################################################

add_patch("opensbi" "opensbi-firmware-secure-boot.patch" ${sm_srcdir}/opensbi sm_patches)
add_custom_target("sm" ALL DEPENDS ${sm_wrkdir_exists} ${sm_patches} "buildroot" WORKING_DIRECTORY ${sm_wrkdir}
  COMMAND ${sm_srcdir}/opensbi/scripts/carray.sh -i ${sm_srcdir}/plat/generic/platform_override_modules.carray > ${sm_srcdir}/plat/generic/platform_override_modules.c  
  COMMAND $(MAKE) -C ${sm_srcdir}/opensbi O=${sm_wrkdir} PLATFORM_DIR=${sm_srcdir}/plat/${platform}
  PATH=${toolchain_dir}:$ENV{PATH} CROSS_COMPILE=${cross_compile}
  FW_PAYLOAD_PATH=${linux_image} FW_PAYLOAD=y PLATFORM_RISCV_XLEN=${BITS}
  PLATFORM_RISCV_ISA=${ISA} PLATFORM_RISCV_ABI=${ABI}
  COMMENT "Building sm"
  )

###############################################################################
## Component: SDK
###############################################################################
set(sdk_wrkdir sdk)
ExternalProject_Add(sdk
  DEPENDS "buildroot_toolchain"
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/sdk
  CMAKE_ARGS -DKEYSTONE_SDK_DIR=${KEYSTONE_SDK_DIR} -DPATH=${toolchain_dir}:$ENV{PATH}
  DOWNLOAD_COMMAND ""
)

###############################################################################
## COMPONENT: tests
###############################################################################
ExternalProject_Add(examples
  DEPENDS sdk
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/examples
  CMAKE_ARGS -DKEYSTONE_SDK_DIR=${KEYSTONE_SDK_DIR} -DPATH=${toolchain_dir}:$ENV{PATH}
  DOWNLOAD_COMMAND ""
  BUILD_COMMAND make examples
  INSTALL_COMMAND ""
)
set_target_properties(examples PROPERTIES EXCLUDE_FROM_ALL YES)
add_custom_target("tests" DEPENDS examples)

###############################################################################
## PATCH
###############################################################################
add_custom_target("patch" DEPENDS ${linux_patches} ${sm_patches})

###############################################################################
## QEMU scripts
###############################################################################

set(scripts ${CMAKE_BINARY_DIR}/scripts)
set(qemu_system ${buildroot_wrkdir}/host/bin//qemu-system-riscv${BITS})
add_custom_command(OUTPUT ${scripts} COMMAND mkdir -p ${scripts})

# if initramfs is false, we need to tell qemu where to find the block device
if(initramfs)
  set(extra_qemu_options "")
else()
  set(extra_qemu_options "\
      -append \"console=ttyS0 ro root=/dev/vda\" \
      -drive file=${buildroot_wrkdir}/images/rootfs.ext2,format=raw,id=hd0 \
      -device virtio-blk-device,drive=hd0 \
  ")
endif()

# generate the qemu runscript, using the above options
string(RANDOM LENGTH 4 ALPHABET 0123456789 qemu_ssh_port)
math(EXPR qemu_ssh_port "3000 + ${qemu_ssh_port}%3000")
configure_file(
  ${CMAKE_SOURCE_DIR}/scripts/run-qemu.sh.in
  ${scripts}/run-qemu.sh
  @ONLY)

configure_file(
  ${CMAKE_SOURCE_DIR}/scripts/test-qemu.sh.in
  ${scripts}/test-qemu.sh
  @ONLY)

add_custom_command(OUTPUT ${scripts}/travis.sh
  DEPENDS ${CMAKE_SOURCE_DIR}/scripts ${scripts}
  COMMAND cp ${CMAKE_SOURCE_DIR}/scripts/travis.sh ${scripts})
add_custom_command(OUTPUT ${scripts}/gdb.sh
  DEPENDS ${CMAKE_SOURCE_DIR}/scripts ${scripts}
  COMMAND cp ${CMAKE_SOURCE_DIR}/scripts/gdb.sh ${scripts})

add_custom_target(
  "tools" ALL
  DEPENDS ${scripts} ${scripts}/run-qemu.sh ${scripts}/test-qemu.sh
  ${scripts}/travis.sh ${scripts}/gdb.sh
  COMMENT "Generating scripts and tools"
)

add_custom_target(
  "run-tests"
  DEPENDS "tests" "tools" "image" "${fw_bin}"
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMAND
    ./scripts/travis.sh ${CMAKE_SOURCE_DIR}/tests
  COMMENT "Running tests"
)
