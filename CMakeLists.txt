cmake_minimum_required(VERSION 3.16)
project(Triangles VERSION 1.0.1)

include(cmake/clang_format.cmake)

# Prepare project libraries
add_subdirectory(lib)
message(STATUS "Collected libs: ${LIBLIST}")

foreach(LIB ${LIBLIST})
  target_include_directories(${LIB} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
  target_include_directories(${LIB} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/${LIB})
  target_compile_features(${LIB} PRIVATE cxx_std_20)
endforeach()

# Prepare project tools
add_subdirectory(tools)
message(STATUS "Collected tools: ${TOOLLIST}")

foreach(TOOL ${TOOLLIST})
  target_include_directories(${TOOL} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
  target_compile_features(${TOOL} PRIVATE cxx_std_20)
  set_target_properties(${TOOL}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )
endforeach()

# Lookup for gtest
find_package(GTest REQUIRED)

add_subdirectory(test)
message(STATUS "Collected tests: ${TESTLIST}")

foreach(TEST ${TESTLIST})
  target_include_directories(${TEST} PRIVATE ${GTEST_INCLUDE_DIRS})
  target_link_libraries(${TEST} PRIVATE ${GTEST_BOTH_LIBRARIES} pthread)

  target_compile_features(${TEST} PRIVATE cxx_std_20)
  target_include_directories(${TEST} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
  set_target_properties(${TEST}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/test"
  )
endforeach()
