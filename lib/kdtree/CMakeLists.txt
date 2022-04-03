add_library(kdtree INTERFACE)
target_sources(kdtree
  INTERFACE ${CMAKE_SOURCE_DIR}/include/kdtree/container.hh
  INTERFACE ${CMAKE_SOURCE_DIR}/include/kdtree/node.hh
)
