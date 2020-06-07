add_custom_target(binaries ALL
  COMMAND
  env CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/scripts/setup_binary.sh
)
