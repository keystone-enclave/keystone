include(${src_dir}/macros.cmake)

set(libmodules              app edge host verifier)
set(enabled_libmodules      app edge host verifier)

# Common
file(GLOB_RECURSE
  COMMON_SOURCE_FILES
  common/*.[c|cpp])

foreach(module ${enabled_libmodules})
    add_subdirectory(${module})
endforeach()

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/common
  DESTINATION ${out_dir}/include)


