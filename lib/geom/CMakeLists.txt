set(GEOMLIBLIST primitives intersection distance kdtree)

foreach(GEOMLIB ${GEOMLIBLIST})
  add_subdirectory(${GEOMLIB})

  target_include_directories(${GEOMLIB} INTERFACE ${CMAKE_SOURCE_DIR}/include)
  target_include_directories(${GEOMLIB} INTERFACE ${CMAKE_SOURCE_DIR}/include/geom)

  target_sources(${GEOMLIB} INTERFACE ${CMAKE_SOURCE_DIR}/include/geom/${GEOMLIB}/${GEOMLIB}.hh)
  format_target(${GEOMLIB} "" "")
endforeach()

set(GEOMLIBLIST "${GEOMLIBLIST}" PARENT_SCOPE)
