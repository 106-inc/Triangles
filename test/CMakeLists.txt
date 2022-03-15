add_subdirectory(unit)
add_subdirectory(e2e)

set(TESTLIST "${TESTLIST}" PARENT_SCOPE)
set(LIT_RUN_CMD "${LIT_RUN_CMD}" PARENT_SCOPE)
