cmake_minimum_required(VERSION 3.10)
include(ExternalProject)

# find program "makeself"
find_program(MAKESELF makeself)

macro(global_set Name Value)
    #  message("set ${Name} to " ${ARGN})
    set(${Name} "${Value}" CACHE STRING "NoDesc" FORCE)
endmacro()

macro(use_riscv_toolchain bits)
  set(cross_compile riscv${bits}-buildroot-linux-gnu-)
  execute_process(
    COMMAND which ${cross_compile}gcc
    OUTPUT_VARIABLE CROSSCOMPILE
    RESULT_VARIABLE ERROR)

  if (NOT "${ERROR}" STREQUAL 0)
    message(FATAL_ERROR "RISCV Toochain is not found")
  endif()

  string(STRIP ${CROSSCOMPILE} CROSSCOMPILE)
  string(LENGTH ${CROSSCOMPILE} CROSSCOMPILE_LEN)
  math(EXPR CROSSCOMPILE_LEN "${CROSSCOMPILE_LEN} - 3")
  string(SUBSTRING ${CROSSCOMPILE} 0 ${CROSSCOMPILE_LEN} CROSSCOMPILE)

  message(STATUS "Tagret tripplet: ${CROSSCOMPILE}")

  set(CC              ${CROSSCOMPILE}gcc)
  set(CXX             ${CROSSCOMPILE}g++)
  set(AS              ${CROSSCOMPILE}as)
  set(LD              ${CROSSCOMPILE}ld)
  set(AR              ${CROSSCOMPILE}ar)
  set(OBJCOPY         ${CROSSCOMPILE}objcopy)
  set(OBJDUMP         ${CROSSCOMPILE}objdump)
  set(STRIP           ${CROSSCOMPILE}strip)
  set(CFLAGS          "-Wall -Werror")

  global_set(CMAKE_C_COMPILER        ${CC}${EXT})
  global_set(CMAKE_ASM_COMPILER      ${CC}${EXT})
  global_set(CMAKE_CXX_COMPILER      ${CXX}${EXT})
  global_set(CMAKE_LINKER            ${LD}${EXT})
  global_set(CMAKE_AR                ${AR}${EXT})
  global_set(CMAKE_OBJCOPY           ${OBJCOPY}${EXT})
  global_set(CMAKE_OBJDUMP           ${OBJDUMP}${EXT})
  global_set(CMAKE_STRIP             ${STRIP}${EXT})
  global_set(CMAKE_C_FLAGS           ${CMAKE_C_FLAGS} ${CFLAGS})

  check_compiler(${CMAKE_C_COMPILER})
  check_compiler(${CMAKE_CXX_COMPILER})

  global_set(CMAKE_C_COMPILER_WORKS      1)
  global_set(CMAKE_CXX_COMPILER_WORKS    1)

  global_set(CMAKE_SYSTEM_NAME    "Linux")

endmacro()

macro(check_compiler target)
  message(STATUS "Check for working C compiler: ${target}")
  execute_process(
    COMMAND ${target} -print-file-name=crt.o
    OUTPUT_FILE OUTPUT
    RESULT_VARIABLE ERROR)

  if ("${ERROR}" STREQUAL 0)
    message(STATUS "Check for working C compiler: ${target} -- works")
  else()
    message(FATAL_ERROR "Check for working C compiler: ${target} -- not working")
  endif()
endmacro()

macro(subdirs_list OUT_VARIABLE DIRWORK)
  file(GLOB children RELATIVE ${DIRWORK} ${DIRWORK}/*)
  set(_subdirs    "")
  foreach(child ${children})
    if (IS_DIRECTORY ${DIRWORK}/${child})
      list(APPEND _subdirs  ${child})
    endif()
  endforeach()
  set(${OUT_VARIABLE} ${_subdirs})
endmacro()

macro(add_files FILE_LIST DIRWORK)
  foreach(file ${ARGN})
    list(APPEND ${FILE_LIST} ${DIRWORK}/${file})
  endforeach()
endmacro()

macro(get_runtime_dir var)
  if (DEFINED ENV{KEYSTONE_RUNTIME})
    set(${var} $ENV{KEYSTONE_RUNTIME})
  elseif (DEFINED KEYSTONE_RUNTIME)
    set(${var} ${KEYSTONE_RUNTIME})
  else()
    message(FATAL_ERROR "Dont know how to find the runtime sources")
  endif()
endmacro()

# CMake macro for Eyrie runtime and Keystone Package
macro(add_eyrie_runtime target_name plugins) # the files are passed via ${ARGN}
  set(runtime_prefix runtime)
  set (eyrie_src ${CMAKE_CURRENT_BINARY_DIR}/${runtime_prefix}/src/eyrie-${target_name})

  separate_arguments(PLUGIN_ARGS UNIX_COMMAND ${plugins})
  set(PLUGIN_FLAGS "")
  foreach(plugin IN ITEMS ${PLUGIN_ARGS})
    string(TOUPPER ${plugin} PLUGIN_UPPER)
    list(APPEND PLUGIN_FLAGS "-D${PLUGIN_UPPER}=ON")
  endforeach()

  list(APPEND PLUGIN_FLAGS "-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}")
  list(APPEND PLUGIN_FLAGS "-DCMAKE_OBJCOPY=${CMAKE_OBJCOPY}")

  if(NOT DEFINED KEYSTONE_EYRIE_RUNTIME)
    get_runtime_dir(KEYSTONE_EYRIE_RUNTIME)
  endif()

  ExternalProject_Add(eyrie-${target_name}
    PREFIX ${runtime_prefix}
    DOWNLOAD_COMMAND rm -rf ${eyrie_src} && cp -ar ${KEYSTONE_EYRIE_RUNTIME} ${eyrie_src}
    CMAKE_ARGS "${PLUGIN_FLAGS}" -DEYRIE_SRCDIR=${KEYSTONE_EYRIE_RUNTIME} -DKEYSTONE_SDK_DIR=${KEYSTONE_SDK_DIR}
    BUILD_IN_SOURCE TRUE
    BUILD_BYPRODUCTS ${eyrie_src}/eyrie-rt ${eyrie_src}/.options_log
    INSTALL_COMMAND "")

  add_custom_target(${target_name} DEPENDS ${ARGN})

  foreach(item IN ITEMS ${ARGN})
    add_custom_command(OUTPUT ${item} DEPENDS eyrie-${target_name} ${eyrie_src}/${item}
      COMMAND cp ${eyrie_src}/${item} ${item})
  endforeach(item)

endmacro(add_eyrie_runtime)

macro(add_keystone_package target_name package_name package_script) # files are passed via ${ARGN}
  set(pkg_dir ${CMAKE_CURRENT_BINARY_DIR}/pkg)
  add_custom_command(OUTPUT ${pkg_dir} COMMAND mkdir ${pkg_dir})

  message(STATUS " * Configuring Keystone package (${target_name})")
  foreach(dep IN ITEMS ${ARGN})
    get_filename_component(filename ${dep} NAME)
    string(CONCAT pkg_file "${pkg_dir}/" "${filename}")
    list(APPEND pkg_files ${pkg_file})

    message(STATUS "   Adding ${filename}")
    add_custom_command(OUTPUT ${pkg_file} DEPENDS ${dep} ${pkg_dir}
      COMMAND cp ${dep} ${pkg_file})
  endforeach(dep)

  message(STATUS "   Package: ${package_name}")
  message(STATUS "   Script: ${package_script}")

  separate_arguments(package_script_raw UNIX_COMMAND ${package_script})
  add_custom_target(${target_name} DEPENDS ${pkg_files}
    COMMAND
      ${MAKESELF} --noprogress ${pkg_dir} ${package_name} \"Keystone Enclave Package\" ${package_script_raw}
    )

endmacro(add_keystone_package)

macro(add_exportable_function target_name scratch_size target_src)
  # First, we need to compile the executable
  add_executable(${target_name}_sym ${target_src})

  # Generate a random address to place the exported function at. Note
  # that this code assumes that the page size to align to is 0x1000.
  string(RANDOM LENGTH 6 ALPHABET 0123456789ABCDEF ${target_name}_addr)
  math(EXPR ${target_name}_addr "0x20000000 + (0x${${target_name}_addr} * 0x1000)"
          OUTPUT_FORMAT HEXADECIMAL)

  target_compile_options(${target_name}_sym PRIVATE
          -ffunction-sections -fdata-sections -mcmodel=medany)

  # Link with gc-sections to strip out functionality we don't need
  get_sdk_dir(SDK_SRCDIR)
  target_link_options(${target_name}_sym PRIVATE --static
          -nostartfiles -e ${target_name} -Wl,--gc-sections
          -T ${SDK_SRCDIR}/src/app/export_link.ld
          -Wl,--section-start=.keystone=${${target_name}_addr})

  # Now strip the program of all its debugging information, and
  # simultaneously objcopy it into a binary format
  add_custom_target(${target_name}_bin
          DEPENDS ${target_name}_sym
          BYPRODUCTS ${target_name}_bin
          COMMAND ${CMAKE_STRIP} -R .rela.dyn -O binary -o ${target_name}_bin ${target_name}_sym)

  # Pad the binary to a multiple of the page size (assumed to be 0x1000 here). Also,
  # append the requested amount of scratch space.
  add_custom_target(${target_name}_pad
          DEPENDS ${target_name}_bin
          BYPRODUCTS ${target_name}_pad
          COMMAND cp ${target_name}_bin ${target_name}_pad &&
                  truncate -s $$\(\(
                            (( $$\(stat --printf="%s" ${target_name}_pad\) + ${scratch_size})
                                / 0x1000 + 1) * 0x1000\)\) ${target_name}_pad)

  # Hexify the stripped binary
  add_custom_target(${target_name}_hex
          DEPENDS ${target_name}_pad
          BYPRODUCTS ${target_name}_hex
          COMMAND cat ${target_name}_pad | xxd -i > ${target_name}_hex)

  # Finally, create the shared library target for the raw data. We make this an
  # OBJECT library, so that the linker symbols __start_keystone_exported and
  # __stop_keystone_exported are defined correctly in the caller
  add_library(${target_name} OBJECT ${SDK_SRCDIR}/src/app/export_wrap.c)
  add_dependencies(${target_name} ${target_name}_hex)

  # These compile definitions parameterize export_wrap.c for a specific
  # exported function, and pass this info on to the caller
  target_compile_definitions(${target_name} PRIVATE
          FUNCNAME=${target_name}
          FNAME=\"${CMAKE_CURRENT_BINARY_DIR}/${target_name}_hex\"
          FBASE=${${target_name}_addr}
          FSCRATCH=${scratch_size})

endmacro(add_exportable_function)
