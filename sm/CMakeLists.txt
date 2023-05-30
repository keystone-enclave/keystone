cmake_minimum_required(VERSION 3.5)

project(keystone_sm)

add_patch("sm/opensbi" "opensbi-firmware-secure-boot.patch"
          ${sm_srcdir}/opensbi sm_patches)

set(carray_script ${sm_srcdir}/opensbi/scripts/carray.sh)
set(carray_src_file ${sm_srcdir}/plat/generic/platform_override_modules.c)

add_custom_command(
  DEPENDS ${sm_srcdir} ${sm_patches}
  OUTPUT ${carray_src_file}
  COMMAND
    ${sm_srcdir}/opensbi/scripts/carray.sh -i
    ${sm_srcdir}/plat/generic/platform_override_modules.carray >
    ${carray_src_file})

file(GLOB_RECURSE sm_sources "${sm_srcdir}/*.c")

set(sm_built_flag ${sm_wrkdir}/.built)

add_custom_command(
  DEPENDS ${carray_src_file} ${sm_sources} "linux"
  OUTPUT ${sm_built_flag}
  COMMAND
    ${CMAKE_MAKE_PROGRAM} -C ${sm_srcdir}/opensbi O=${sm_wrkdir}
    PLATFORM_DIR=${sm_srcdir}/plat/${platform}
    CROSS_COMPILE=riscv${BITS}-unknown-elf- FW_PAYLOAD_PATH=${linux_image}
    FW_PAYLOAD=y PLATFORM_RISCV_XLEN=${BITS} PLATFORM_RISCV_ISA=${ISA}
    PLATFORM_RISCV_ABI=${ABI}
  COMMAND ${CMAKE_COMMAND} -E touch ${sm_built_flag}
  COMMENT "Building sm")

add_custom_target("sm" ALL DEPENDS ${sm_built_flag})
