cmake_minimum_required(VERSION 3.5)
project(keystone C)
include(ProcessorCount)
set(CMAKE_VERBOSE_MAKEFILE ON)

###############################################################################
## MACROS
###############################################################################

# add patch macro
macro(add_patch submodule patch working_directory patch_list)
  add_custom_command(OUTPUT ${patch}.applied
    WORKING_DIRECTORY ${working_directory}
    COMMAND patch --forward -p0 < ${patchdir}/${submodule}/${patch}
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

if(NOT DEFINED ENV{RISCV})
	message(FATAL_ERROR "set RISCV environment variable. \n Try `cd ${CMAKE_SOURCE_DIR}; source source.sh`")
endif()

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

set(ISA rv${BITS}imafdc)
set(confdir ${CMAKE_SOURCE_DIR}/conf)
set(patchdir ${CMAKE_SOURCE_DIR}/patches)
set(cross_compile riscv${BITS}-unknown-linux-gnu-)

set(bootrom_srcdir ${CMAKE_SOURCE_DIR}/bootrom)
set(bootrom_wrkdir ${CMAKE_BINARY_DIR}/bootrom.build)
set(qemu_wrkdir ${CMAKE_SOURCE_DIR}/qemu/build)
set(qemu_srcdir ${CMAKE_SOURCE_DIR}/qemu)
set(sm_srcdir ${CMAKE_SOURCE_DIR}/sm)
mkdir(sm_wrkdir ${CMAKE_BINARY_DIR}/sm.build)
set(buildroot_srcdir ${CMAKE_SOURCE_DIR}/buildroot)
set(buildroot_wrkdir ${CMAKE_BINARY_DIR}/buildroot.build)
set(buildroot_config ${confdir}/qemu_riscv${BITS}_virt_defconfig)
set(overlay_dir ${CMAKE_BINARY_DIR}/overlay)
set(overlay_root ${overlay_dir}/root)

if(firesim)
  message(STATUS "Using Linux defconfig: ${linux_defconfig}")
	set(initramfs true)
elseif(sifive)
  message(STATUS "SiFive Unleashed configs and patches. Forcing initramfs=y")
	set(linux_defconfig ${confdir}/linux64-sifive-defconfig)
  message(STATUS "Using Linux defconfig: ${linux_defconfig}")
	set(initramfs true)
elseif(cva6)
  message(STATUS "CVA6 configs and patches. Forcing initramfs=y")
	set(linux_defconfig ${confdir}/linux64-cva6-defconfig)
  message(STATUS "Using Linux defconfig: ${linux_defconfig}")
  set(buildroot_config ${confdir}/riscv${BITS}_cva6_defconfig)
  message(STATUS "Using buildroot defconfig: ${buildroot_config}")
	set(initramfs true)
endif()

if(NOT DEFINED linux_defconfig)
	set(linux_defconfig ${confdir}/linux${BITS}-defconfig)
endif()

set(linux_srcdir ${CMAKE_SOURCE_DIR}/linux)
mkdir(linux_wrkdir ${CMAKE_BINARY_DIR}/linux.build)
set(linux_symvers ${linux_wrkdir}/Modules.symvers)
set(linux_image ${linux_wrkdir}/arch/riscv/boot/Image)
set(driver_srcdir ${CMAKE_SOURCE_DIR}/linux-keystone-driver)
set(driver_wrkdir ${CMAKE_BINARY_DIR}/linux-keystone-driver.build)
set(fw_elf ${sm_wrkdir}/platform/${platform}/firmware/fw_jump.elf)
set(fw_bin ${sm_wrkdir}/platform/${platform}/firmware/fw_jump.bin)
set(initramfs_sysroot ${CMAKE_BINARY_DIR}/initramfs-sysroot)


###############################################################################
## COMPONENT: QEMU
###############################################################################

set(qemu_system ${qemu_wrkdir}/riscv${BITS}-softmmu/qemu-system-riscv${BITS})
add_patch("qemu" "qemu-rom.patch" ${qemu_srcdir} qemu_patches)

add_custom_target("qemu" ALL DEPENDS ${qemu_system})
add_custom_command(OUTPUT ${qemu_system}
  COMMAND $(MAKE) -C ${qemu_srcdir}
  DEPENDS ${qemu_wrkdir}/config-host.mak
  ${qemu_patches}
  COMMENT "Building QEMU"
)
add_custom_command(OUTPUT ${qemu_wrkdir}/config-host.mak
  WORKING_DIRECTORY ${qemu_srcdir}
  COMMAND ./configure --target-list=riscv${BITS}-softmmu,riscv${BITS}-linux-user
  COMMENT "Configuring QEMU"
)


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
add_custom_target("buildroot" ALL DEPENDS ${buildroot_srcdir} ${buildroot_wrkdir}/.config ${overlay_root} ${buildroot_wrkdir}
  COMMAND $(MAKE) -s -C ${buildroot_srcdir} RISCV=$ENV{RISCV} PATH=$ENV{PATH} O=${buildroot_wrkdir}
  COMMENT "Building buildroot"
)

string(REPLACE "/" "\\/" overlay_dir_stripped ${overlay_dir})
add_custom_command(DEPENDS ${buildroot_config} OUTPUT ${buildroot_wrkdir}/.config
  COMMAND mkdir -p ${buildroot_wrkdir}
  COMMAND cp ${buildroot_config} ${buildroot_wrkdir}/.config
  COMMAND sed \"s/^BR2_ROOTFS_OVERLAY=.*/BR2_ROOTFS_OVERLAY=\\\"${overlay_dir_stripped}\\\"/g\" -i ${buildroot_wrkdir}/.config
  COMMAND $(MAKE) -s -C ${buildroot_srcdir} RISCV=$ENV{RISCV} PATH=$ENV{PATH}
    O=${buildroot_wrkdir} CROSS_COMPILE=${cross_compile} olddefconfig
  COMMENT "Configuring buildroot (overlay = ${overlay_dir})"
)


###############################################################################
## component: bootrom
###############################################################################

add_custom_command(OUTPUT ${bootrom_wrkdir} COMMAND mkdir -p ${bootrom_wrkdir})
add_custom_target("bootrom-sync" DEPENDS ${bootrom_srcdir} ${bootrom_wrkdir}
  COMMAND rsync -r ${bootrom_srcdir}/ ${bootrom_wrkdir})
add_custom_target("bootrom" ALL DEPENDS ${bootrom_wrkdir} ${bootrom_srcdir} "bootrom-sync"
  COMMAND $(MAKE) -C ${bootrom_wrkdir} O=${bootrom_wrkdir} march=${ISA} mabi=${ABI} CC=riscv${BITS}-unknown-elf-gcc
    OBJCOPY=riscv${BITS}-unknown-elf-objcopy
  COMMENT "Building bootrom"
)


###############################################################################
## COMPONENT: linux kernel
###############################################################################

add_custom_command(OUTPUT ${linux_wrkdir}/.config DEPENDS ${linux_defconfig} ${linux_wrkdir_exists}
  COMMAND cp ${linux_defconfig} ${linux_wrkdir}/.config
  COMMAND $(MAKE) -C ${linux_srcdir} O=${linux_wrkdir} CROSS_COMPILE=${cross_compile} ARCH=riscv olddefconfig
  COMMENT "Configuring linux"
)
add_custom_target("linux-config" DEPENDS ${linux_wrkdir}/.config)

# if(firesim)
#   add_patch("linux" "linux${BITS}.firesim.patch" ${linux_srcdir} linux_patches)
# elseif(sifive)
#   add_patch("linux" "linux${BITS}.sifive.patch" ${linux_srcdir} linux_patches)
# else()
#   add_patch("linux" "linux${BITS}.patch" ${linux_srcdir} linux_patches)
# endif()

if(initramfs)
  # linux-initramfs
  execute_process(COMMAND id -u OUTPUT_VARIABLE uid)
  string(STRIP ${uid} uid)
  execute_process(COMMAND id -g OUTPUT_VARIABLE gid)
  string(STRIP ${gid} gid)
  add_custom_command(OUTPUT ${initramfs_sysroot} COMMAND mkdir -p ${initramfs_sysroot})
  add_custom_command(OUTPUT ${linux_image} DEPENDS ${initramfs_sysroot} ${linux_srcdir} "linux-symvers" "buildroot" ${buildroot_wrkdir}/images/rootfs.tar
    COMMAND tar -xpf ${buildroot_wrkdir}/images/rootfs.tar -C ${initramfs_sysroot} --exclude ./dev --exclude ./usr/share/locale
    COMMAND echo "::sysinit:/bin/mount -t devtmpfs devtmpfs /dev" >> ${initramfs_sysroot}/etc/inittab
    COMMAND $(MAKE) -C ${linux_srcdir}
      O=${linux_wrkdir} CONFIG_INITRAMFS_SOURCE="${confdir}/initramfs.txt ${initramfs_sysroot}"
      CONFIG_INITRAMFS_ROOT_UID=${uid} CONFIG_INITRAMFS_ROOT_GID=${gid}
      CONFIG_DEVTMPFS=y CONFIG_DEVTMPFS_MOUNT=y
      CROSS_COMPILE=${cross_compile} ARCH=riscv
    COMMENT "Building linux (initramfs)"
  )
else()
  add_custom_command(OUTPUT ${linux_image} DEPENDS ${linux_srcdir} "linux-symvers"
    COMMAND $(MAKE) -C ${linux_srcdir} O=${linux_wrkdir} CROSS_COMPILE=${cross_compile} ARCH=riscv
    COMMENT "Building linux"
  )
endif()

add_custom_command(OUTPUT ${linux_symvers} DEPENDS ${linux_srcdir} "linux-config" ${linux_patches}
  COMMAND $(MAKE) -C ${linux_srcdir} O=${linux_wrkdir} CROSS_COMPILE=${cross_compile} ARCH=riscv modules
  COMMENT "Building linux symvers"
)
add_custom_target("linux-symvers" DEPENDS ${linux_symvers})
add_custom_target("linux" ALL DEPENDS ${linux_image})


###############################################################################
## COMPONENT: linux driver
###############################################################################

add_custom_command(OUTPUT ${driver_wrkdir} COMMAND mkdir -p ${driver_wrkdir})
add_custom_target("driver-sync" DEPENDS ${driver_srcdir} ${driver_wrkdir}
	COMMAND rsync -r ${driver_srcdir}/ ${driver_wrkdir})
add_custom_target("driver" ALL DEPENDS ${driver_srcdir} ${linux_srcdir} "linux-symvers" "driver-sync"
  COMMAND $(MAKE) -C ${linux_wrkdir} O=${linux_wrkdir} CROSS_COMPILE=${cross_compile} ARCH=riscv
    M=${driver_wrkdir} modules
  COMMENT "Building driver"
)


###############################################################################
## COMPONENT: security monitor (sm)
###############################################################################

add_patch("sm/opensbi" "opensbi-firmware-secure-boot.patch" ${sm_srcdir}/opensbi sm_patches)
add_custom_target("sm" ALL DEPENDS "linux" ${sm_wrkdir_exists} ${sm_patches} WORKING_DIRECTORY ${sm_wrkdir}
  COMMAND ${sm_srcdir}/opensbi/scripts/carray.sh -i ${sm_srcdir}/plat/generic/platform_override_modules.carray > ${sm_srcdir}/plat/generic/platform_override_modules.c  
  COMMAND $(MAKE) -C ${sm_srcdir}/opensbi O=${sm_wrkdir} PLATFORM_DIR=${sm_srcdir}/plat/${platform}
  CROSS_COMPILE=riscv${BITS}-unknown-elf- FW_PAYLOAD_PATH=${linux_image} FW_PAYLOAD=y PLATFORM_RISCV_XLEN=${BITS}
  PLATFORM_RISCV_ISA=${ISA} PLATFORM_RISCV_ABI=${ABI}
  COMMENT "Building sm"
  )

###############################################################################
## COMPONENT: tests
###############################################################################
set(example_wrkdir examples)
add_subdirectory(sdk/examples ${example_wrkdir})
set_target_properties(examples PROPERTIES EXCLUDE_FROM_ALL YES)

add_custom_target("tests" DEPENDS examples)

###############################################################################
## COMPONENT: image
###############################################################################

add_custom_target("image-deps" DEPENDS "driver" ${overlay_root}
  COMMAND find ${driver_wrkdir} -name "*.ko" -exec cp {} ${overlay_root} \\\\;
)
add_custom_target("image" DEPENDS "buildroot" "sm"
  COMMENT "Generating image"
)

add_dependencies("buildroot" "image-deps")

###############################################################################
## PATCH
###############################################################################
add_custom_target("patch" DEPENDS ${qemu_patches} ${linux_patches} ${sm_patches})

###############################################################################
## QEMU scripts
###############################################################################

set(scripts ${CMAKE_BINARY_DIR}/scripts)
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
