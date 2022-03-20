set(TOOLLIST lvl1q)

foreach(TOOL ${TOOLLIST})
  add_subdirectory(${TOOL})
endforeach()

set(TOOLLIST "${TOOLLIST}" PARENT_SCOPE)
