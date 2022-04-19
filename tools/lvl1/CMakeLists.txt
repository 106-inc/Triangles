add_executable(lvl1 main.cc)
target_link_libraries(lvl1 primitives intersection)
format_target(lvl1 ${CMAKE_CURRENT_SOURCE_DIR} main.cc)
