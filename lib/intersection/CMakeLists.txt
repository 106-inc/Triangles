add_library(intersection INTERFACE)
target_sources(intersection
  INTERFACE ${CMAKE_SOURCE_DIR}/include/intersection/detail.hh
)
