set(LIBLIST primitives intersection distance kdtree)

foreach(LIB ${LIBLIST})
  add_subdirectory(${LIB})
  target_sources(${LIB} INTERFACE ${CMAKE_SOURCE_DIR}/include/${LIB}/${LIB}.hh)
  format_target(${LIB} "" "")
endforeach()

set(LIBLIST "${LIBLIST}" PARENT_SCOPE)
