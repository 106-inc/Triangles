set(SRC vector.cc line.cc plane.cc triangle.cc)
add_library(primitives ${SRC})
format_target(primitives ${CMAKE_CURRENT_SOURCE_DIR} ${SRC})
