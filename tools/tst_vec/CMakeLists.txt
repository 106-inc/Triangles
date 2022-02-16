set(SRC main.cc)
add_executable(tst_vec main.cc)
target_link_libraries(tst_vec primitives)
format_target(tst_vec ${CMAKE_CURRENT_SOURCE_DIR} ${SRC})
