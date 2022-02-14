set(SRC main.cc)
set(TSTLIB vector_tst)

add_executable(${TSTLIB} ${SRC})
target_link_libraries(${TSTLIB} PRIVATE primitives)

format_target(${TSTLIB} ${CMAKE_CURRENT_SOURCE_DIR} ${SRC})
