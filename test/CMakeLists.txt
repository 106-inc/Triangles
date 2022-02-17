set(TESTLIST vector_tst line_tst plane_tst triangle_tst)

foreach(TEST ${TESTLIST})
  add_subdirectory(${TEST})
endforeach()

set(TESTLIST "${TESTLIST}" PARENT_SCOPE)
