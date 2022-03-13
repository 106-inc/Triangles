set(SRC main.cc)
set(TSTLIB vec3_tst)

add_executable(${TSTLIB} ${SRC})
target_link_libraries(${TSTLIB} PRIVATE primitives)

format_target(${TSTLIB} ${CMAKE_CURRENT_SOURCE_DIR} ${SRC})
