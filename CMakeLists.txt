cmake_minimum_required(VERSION 3.10)
project(keystone C)
include(ProcessorCount)

if(NOT DEFINED ENV{RISCV})
  message(FATAL_ERROR "set RISCV environment variable")
endif()

if(Debug)
  message(STATUS "debug")
  set(CFLAGS -g)
endif()

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

set(bootrom_wrkdir ${CMAKE_BINARY_DIR}/bootrom${BITS})
set(bootrom_srcdir ${CMAKE_SOURCE_DIR}/bootrom)
set(qemu_wrkdir ${CMAKE_SOURCE_DIR}/qemu)
set(qemu_srcdir ${CMAKE_SOURCE_DIR}/qemu)
set(sm_srcdir ${CMAKE_SOURCE_DIR}/riscv-pk)
set(sm_wrkdir ${CMAKE_BINARY_DIR}/riscv-pk${BITS})
set(buildroot_srcdir ${CMAKE_SOURCE_DIR}/buildroot)
set(buildroot_wrkdir ${CMAKE_BINARY_DIR}/buildroot${BITS})
set(buildroot_config ${confdir}/qemu_riscv${BITS}_virt_defconfig)
set(overlay_dir ${CMAKE_BINARY_DIR}/overlay)
set(overlay_root ${overlay_dir}/root)
set(linux_wrkdir ${CMAKE_BINARY_DIR}/linux${BITS})
set(linux_srcdir ${CMAKE_SOURCE_DIR}/linux)
set(linux_defconfig ${confdir}/linux-v5.0-defconfig-rv${BITS})
set(linux_vmlinux ${linux_wrkdir}/vmlinux)
set(linux_vmlinux_stripped ${linux_wrkdir}/vmlinux-stripped)
set(driver_srcdir ${CMAKE_SOURCE_DIR}/linux-keystone-driver)
set(driver_wrkdir ${CMAKE_BINARY_DIR}/linux-keystone-driver${BITS})
set(tests_srcdir ${CMAKE_SOURCE_DIR}/tests)


# QEMU
add_custom_target("qemu" ALL COMMAND $(MAKE) -C ${qemu_srcdir} DEPENDS "qemu-config")
add_custom_target("qemu-config" DEPENDS ${qemu_srcdir}/ ${CMAKE_SOURCE_DIR}/patches/qemu/
  WORKING_DIRECTORY ${qemu_srcdir}
  COMMAND patch --forward -p0 < ${patchdir}/qemu/qemu-pmp-bug.patch || true
  COMMAND patch --forward -p0 < ${patchdir}/qemu/qemu-secure-boot.patch || true
  COMMAND ./configure --target-list=riscv${BITS}-softmmu,riscv${BITS}-linux-user
)

# buildroot
add_custom_command(OUTPUT ${overlay_root} COMMAND mkdir -p ${overlay_root})
add_custom_command(OUTPUT ${overlay_dir} COMMAND mkdir -p ${overlay_dir})
add_custom_command(OUTPUT ${buildroot_wrkdir} COMMAND mkdir -p ${buildroot_wrkdir})
add_custom_target("buildroot" ALL DEPENDS ${buildroot_srcdir} ${buildroot_wrkdir}/.config ${overlay_root} ${buildroot_wrkdir}
  COMMAND $(MAKE) -s -C ${buildroot_srcdir} RISCV=$ENV{RISCV} PATH=$ENV{PATH} O=${buildroot_wrkdir}
)

string(REPLACE "/" "\\/" overlay_dir_stripped ${overlay_dir})
add_custom_command(DEPENDS ${buildroot_config} OUTPUT ${buildroot_wrkdir}/.config
  COMMAND mkdir -p ${buildroot_wrkdir}
  COMMAND cp ${buildroot_config} ${buildroot_wrkdir}/.config
  COMMAND sed \"s/^BR2_ROOTFS_OVERLAY=.*/BR2_ROOTFS_OVERLAY=\\\"${overlay_dir_stripped}\\\"/g\" -i ${buildroot_wrkdir}/.config
  COMMAND $(MAKE) -s -C ${buildroot_srcdir} RISCV=$ENV{RISCV} PATH=$ENV{PATH}
    O=${buildroot_wrkdir} CROSS_COMPILE=${cross_compile} olddefconfig
)

# boot ROM
add_custom_command(OUTPUT ${bootrom_wrkdir} COMMAND mkdir -p ${bootrom_wrkdir})
add_custom_target("bootrom" ALL
  COMMAND $(MAKE) -C ${bootrom_srcdir} O=${bootrom_wrkdir}
  DEPENDS ${bootrom_wrkdir} ${bootrom_srcdir})

# linux
add_custom_command(OUTPUT ${linux_wrkdir} COMMAND mkdir -p ${linux_wrkdir})
add_custom_target("linux" ALL DEPENDS ${linux_srcdir} ${linux_wrkdir}/.config
  COMMAND cd ${linux_srcdir} \; patch --forward -p0 < ${patchdir}/linux/linux.patch || true
  COMMAND $(MAKE) -C ${linux_srcdir} O=${linux_wrkdir} CROSS_COMPILE=${cross_compile} ARCH=riscv vmlinux
  COMMAND ${cross_compile}strip -o ${linux_vmlinux_stripped} ${linux_vmlinux}
)

add_custom_command(OUTPUT ${linux_wrkdir}/.config DEPENDS ${linux_defconfig}
  COMMAND mkdir -p ${linux_wrkdir}
  COMMAND cp ${linux_defconfig} ${linux_wrkdir}/.config
  COMMAND $(MAKE) -C ${linux_srcdir} O=${linux_wrkdir} ARCH=riscv olddefconfig
)

# linux module
add_custom_target("driver" ALL DEPENDS ${driver_srcdir} ${linux_srcdir} ${linux_wrkdir}
  COMMAND mkdir -p ${driver_wrkdir}
  COMMAND $(MAKE) -C ${linux_srcdir} O=${linux_wrkdir} CROSS_COMPILE=${cross_compile} ARCH=riscv
    M=${driver_srcdir} modules
  COMMAND $(MAKE) -C ${linux_srcdir} O=${linux_wrkdir} CROSS_COMPILE=${cross_compile} ARCH=riscv
    M=${driver_srcdir} modules_install INSTALL_MOD_PATH=${driver_wrkdir}
)

# bbl + sm
add_custom_command(OUTPUT ${sm_wrkdir} COMMAND mkdir -p ${sm_wrkdir})
add_custom_target("sm" ALL DEPENDS ${sm_srcdir} ${sm_wrkdir} ${linux_vmlinux_stripped} WORKING_DIRECTORY ${sm_wrkdir}
  COMMAND ${sm_srcdir}/configure --enable-sm --host=riscv${BITS}-unknown-linux-gnu --with-payload=${linux_vmlinux_stripped}
    --enable-logo --with-logo=${confdir}/sifive_logo.txt --with-target-platform=default
    CFLAGS=${CFLAGS}
  COMMAND env CFLAGS='-g -mabi=${ABI} -march=${ISA}' $(MAKE) -C ${sm_wrkdir}
)
add_dependencies("sm" "linux")
add_dependencies("driver" "linux")

add_custom_target("tests" DEPENDS "driver" ${overlay_root} ${tests_srcdir}
  COMMAND OUTPUT_DIR=${overlay_root} ${tests_srcdir}/tests/vault.sh
  COMMAND find ${driver_wrkdir} -name "*.ko" -exec cp {} ${overlay_root} \\\\;
)

add_custom_target("image" DEPENDS "sm" "tests" ${buildroot_srcdir} ${buildroot_wrkdir}/.config ${overlay_root}
  COMMAND $(MAKE) -s -C ${buildroot_srcdir} RISCV=$ENV{RISCV} PATH=$ENV{PATH} O=${buildroot_wrkdir}
)

# scripts
set(scripts ${CMAKE_BINARY_DIR}/scripts)
add_custom_command(OUTPUT ${scripts} COMMAND mkdir -p ${scripts})
add_custom_command(OUTPUT ${scripts}/run-qemu.sh
  WORKING_DIRECTORY ${scripts}
  COMMAND echo "\
    ${qemu_wrkdir}/riscv${BITS}-softmmu/qemu-system-riscv${BITS}\
    -m 2G \
    -nographic \
    -machine virt \
    -bios ${bootrom_wrkdir}/bootrom.bin \
    -kernel ${sm_wrkdir}/bbl \
    -append \"console=ttyS0 ro root=/dev/vda\" \
    -drive file=${buildroot_wrkdir}/images/rootfs.ext2,format=raw,id=hd0 \
    -device virtio-blk-device,drive=hd0 \
    -netdev user,id=net0,net=192.168.100.1/24,dhcpstart=192.168.100.128,hostfwd=tcp::\$\{HOST_PORT\}-:22 \
    -device virtio-net-device,netdev=net0" > run-qemu.sh
    VERBATIM
  COMMAND
    chmod +x run-qemu.sh
  )
add_custom_command(OUTPUT ${scripts}/test-qemu.sh DEPENDS ${CMAKE_SOURCE_DIR}/scripts
  COMMAND cp ${CMAKE_SOURCE_DIR}/scripts/test-qemu.sh ${scripts})
add_custom_command(OUTPUT ${scripts}/travis.sh DEPENDS ${CMAKE_SOURCE_DIR}/scripts
  COMMAND cp ${CMAKE_SOURCE_DIR}/scripts/travis.sh ${scripts})

add_custom_target(
  "all-scripts" ALL
  DEPENDS ${scripts} ${scripts}/run-qemu.sh ${scripts}/test-qemu.sh ${scripts}/travis.sh
)

add_custom_target(
  "run-tests"
  DEPENDS "all-scripts"
  COMMAND
    ./scripts/travis.sh ${CMAKE_SOURCE_DIR}/tests
)
