set(SRC vector.cc)

add_library(geom ${SRC})
# target_include_directories(geom PUBLIC ${CMAKE_PROJECT_DIR}/include/geom)

format_target(geom ${CMAKE_CURRENT_SOURCE_DIR} ${SRC})
