add_library(primitives INTERFACE)
target_sources(primitives
  INTERFACE ${CMAKE_SOURCE_DIR}/include/primitives/boundbox.hh
  INTERFACE ${CMAKE_SOURCE_DIR}/include/primitives/common.hh
  INTERFACE ${CMAKE_SOURCE_DIR}/include/primitives/line.hh
  INTERFACE ${CMAKE_SOURCE_DIR}/include/primitives/plane.hh
  INTERFACE ${CMAKE_SOURCE_DIR}/include/primitives/triangle.hh
  INTERFACE ${CMAKE_SOURCE_DIR}/include/primitives/vec2.hh
  INTERFACE ${CMAKE_SOURCE_DIR}/include/primitives/vec3.hh
)