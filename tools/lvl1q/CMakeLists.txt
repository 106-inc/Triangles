add_executable(lvl1q main.cc) # q means quadratic
target_link_libraries(lvl1q PRIVATE primitives intersection)
format_target(lvl1q ${CMAKE_CURRENT_SOURCE_DIR} main.cc)
  