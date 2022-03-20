configure_file(lit.site.cfg.py.in lit.site.cfg.py @ONLY)

set(LIT_RUN_CMD "${CMAKE_SOURCE_DIR}/test/e2e/my-lit.py" "${CMAKE_BINARY_DIR}/test/e2e" -v --time-tests)
set(LIT_RUN_CMD "${LIT_RUN_CMD}" PARENT_SCOPE)

add_custom_target(check-e2e
  COMMAND python3 ${LIT_RUN_CMD}
  DEPENDS lvl1q
)
