set(LIBLIST primitives intersection)

foreach(LIB ${LIBLIST})
  add_subdirectory(${LIB})
  format_target(${LIB} "" "")
endforeach()

set(LIBLIST "${LIBLIST}" PARENT_SCOPE)
