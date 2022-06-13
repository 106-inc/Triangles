cmake_minimum_required(VERSION 3.16)
project(Triangles VERSION 1.0.1)

# indicate the documentation build
option(BUILD_DOC "Build documentation" ON)
add_subdirectory(docs)

# Include formatter
include(cmake/clang_format.cmake)

# Include compiler flags
include(cmake/comp_flags.cmake)

# Prepare project libraries
add_subdirectory(lib)
message(STATUS "Collected libs: ${GEOMLIBLIST}")

foreach(LIB ${GEOMLIBLIST})
  apply_compiler_flags(${LIB} INTERFACE)
  target_compile_features(${LIB} INTERFACE cxx_std_20)
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

  apply_compiler_flags(${TOOL} PRIVATE)
endforeach()

# indicate the tests build
option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
  # Lookup for gtest
  find_package(GTest REQUIRED)

  add_subdirectory(test)
  message(STATUS "Collected tests: ${TESTLIST}")

  foreach(TEST ${TESTLIST})
    target_include_directories(${TEST} PRIVATE ${GTEST_INCLUDE_DIRS})
    target_link_libraries(${TEST} PRIVATE ${GTEST_BOTH_LIBRARIES} pthread)
    target_link_libraries(${TEST} PRIVATE ${GEOMLIBLIST})

    target_compile_features(${TEST} PRIVATE cxx_std_20)
    target_include_directories(${TEST} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    set_target_properties(${TEST}
      PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/test"
    )
  endforeach()

  # Test running stuff
  enable_testing()

  # Unit tests
  foreach(TEST ${TESTLIST})
    add_test(NAME ${TEST} COMMAND ${TEST} --gtest_color=yes)
    set_property(TEST ${TEST} PROPERTY LABELS unit)
  endforeach()

  # End to end tests
  add_test(NAME e2e COMMAND python3 ${LIT_RUN_CMD})
  set_property(TEST e2e PROPERTY LABELS e2e)
endif()
