include(${src_dir}/macros.cmake)

set(libmodules              app edge host verifier)
set(enabled_libmodules      app edge host verifier)

foreach(module ${enabled_libmodules})
    add_subdirectory(${module})
endforeach()
