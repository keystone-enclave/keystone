macro(global_set Name Value)
    #  message("set ${Name} to " ${ARGN})
    set(${Name} "${Value}" CACHE STRING "NoDesc" FORCE)
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

# CMake macro for Eyrie runtime and Keystone Package
macro(add_eyrie_runtime target_name tag plugins) # the files are passed via ${ARGN}
  set(runtime_prefix runtime)
  set (eyrie_src ${CMAKE_CURRENT_BINARY_DIR}/${runtime_prefix}/src/eyrie-${target_name})

  ExternalProject_Add(eyrie-${target_name}
    PREFIX ${runtime_prefix}
    GIT_REPOSITORY https://github.com/keystone-enclave/keystone-runtime
    GIT_TAG ${tag}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ./build.sh ${plugins}
    BUILD_IN_SOURCE TRUE
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

  add_custom_target(${target_name} DEPENDS ${pkg_files}
    COMMAND
      ${MAKESELF} ${pkg_dir} ${package_name} "Keystone Enclave Package" "${package_script}"
    VERBATIM
    )

endmacro(add_keystone_package)
