configure_file(lit.site.cfg.py.in lit.site.cfg.py @ONLY)

add_custom_target(check-e2e
  COMMAND python3 "${CMAKE_SOURCE_DIR}/test/e2e/my-lit.py" "${CMAKE_CURRENT_BINARY_DIR}" -v
  DEPENDS lvl1q
)
