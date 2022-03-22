file(GLOB_RECURSE TESTSRCLIST CONFIGURE_DEPENDS "*.cc")
set(TESTLIST)

foreach(TESTSRC ${TESTSRCLIST})
  get_filename_component(TESTNAME ${TESTSRC} NAME_WE)
  set(TEST ${TESTNAME}_tst)
  add_executable(${TEST} ${TESTSRC})

  get_filename_component(TESTNAMECC ${TESTSRC} NAME)
  format_target(${TEST} ${CMAKE_CURRENT_SOURCE_DIR} ${TESTNAMECC})

  LIST(APPEND TESTLIST ${TEST})
endforeach()

set(TESTLIST "${TESTLIST}" PARENT_SCOPE)
