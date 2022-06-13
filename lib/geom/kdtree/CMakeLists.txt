add_library(kdtree INTERFACE)
target_link_libraries(kdtree INTERFACE primitives)
target_sources(kdtree
  INTERFACE ${CMAKE_SOURCE_DIR}/include/geom/kdtree/container.hh
  INTERFACE ${CMAKE_SOURCE_DIR}/include/geom/kdtree/node.hh
)
