set(SRC vector.cc)

add_library(Geom ${SRC})
target_include_directories(Geom PUBLIC ${CMAKE_PROJECT_DIR}/include/Geom)

format_target(Geom ${CMAKE_CURRENT_SOURCE_DIR} ${SRC})
