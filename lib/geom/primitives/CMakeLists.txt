add_library(primitives INTERFACE)
target_sources(primitives
  INTERFACE ${CMAKE_SOURCE_DIR}/include/geom/primitives/boundbox.hh
  INTERFACE ${CMAKE_SOURCE_DIR}/include/geom/primitives/common.hh
  INTERFACE ${CMAKE_SOURCE_DIR}/include/geom/primitives/line.hh
  INTERFACE ${CMAKE_SOURCE_DIR}/include/geom/primitives/plane.hh
  INTERFACE ${CMAKE_SOURCE_DIR}/include/geom/primitives/triangle.hh
  INTERFACE ${CMAKE_SOURCE_DIR}/include/geom/primitives/vec2.hh
  INTERFACE ${CMAKE_SOURCE_DIR}/include/geom/primitives/vec3.hh
)