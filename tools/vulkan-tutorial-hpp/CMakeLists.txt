set(TOOL vulkan-tutorial-hpp)

add_executable(${TOOL} main.cc)
format_target(${TOOL} ${CMAKE_CURRENT_SOURCE_DIR} main.cc)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

target_include_directories(${TOOL} PRIVATE ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${TOOL} PRIVATE ${Vulkan_LIBRARIES} glfw)
# target_compile_definitions(${TOOL} PRIVATE VULKAN_HPP_NO_STRUCT_CONSTRUCTORS)

add_custom_command(TARGET ${TOOL} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/tools/vulkan-tutorial-hpp/shaders
                           ${CMAKE_BINARY_DIR}/bin/shaders)
