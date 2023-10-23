add_executable(eapp ${CMAKE_CURRENT_SOURCE_DIR}/vta.cc)
target_link_libraries(eapp "-static" vta-shared lzma memory graph_executor common vta ${model}
        ${model}-ops ${KEYSTONE_LIB_EAPP} m)
vta_default_dirs(eapp)
add_vta_example(direct eapp)