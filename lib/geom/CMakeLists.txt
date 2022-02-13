set(SRC vector.cc line.cc)

add_library(geom ${SRC})

format_target(geom ${CMAKE_CURRENT_SOURCE_DIR} ${SRC})
