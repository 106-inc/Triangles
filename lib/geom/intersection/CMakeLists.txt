add_library(intersection INTERFACE)
target_link_libraries(intersection INTERFACE primitives distance)
target_sources(intersection
  INTERFACE ${CMAKE_SOURCE_DIR}/include/geom/intersection/detail.hh
)
