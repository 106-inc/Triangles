add_library(utils INTERFACE)
target_sources(utils
  INTERFACE ${CMAKE_SOURCE_DIR}/include/utils/utils.hh
)
