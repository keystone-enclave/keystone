include(${src_dir}/macros.cmake)

set(libmodules              app edge host verifier)
set(enabled_libmodules      app edge host verifier)

foreach(module ${enabled_libmodules})
    add_subdirectory(${module})
endforeach()

file(GLOB_RECURSE
  CHECK_CXX_SOURCE_FILES
  host/src/*.cpp host/include/*.hpp
)

find_program(CLANG_FORMAT "clang-format")
find_program(CPPLINT "cpplint")

if(CLANG_FORMAT AND CPPLINT)
  ADD_CUSTOM_TARGET(
    format
    COMMAND
    ${CLANG_FORMAT}
    -i
    -style=file
    ${CHECK_CXX_SOURCE_FILES}
    COMMAND
    ${CPPLINT}
    ${CHECK_CXX_SOURCE_FILES}
    COMMENT "Auto-formatting"
  )
endif()
