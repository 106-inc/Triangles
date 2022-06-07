set(TOOLLIST lvl1q lvl1) # vulkan-tutorial-hpp

foreach(TOOL ${TOOLLIST})
  add_subdirectory(${TOOL})
endforeach()

set(TOOLLIST "${TOOLLIST}" PARENT_SCOPE)
