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
