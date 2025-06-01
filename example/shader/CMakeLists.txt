project(shader CXX)
add_executable(shader src/main.cpp)
target_link_libraries(shader PRIVATE Core Graphic extensions)

configure_binary_output(shader)

add_custom_command(TARGET shader POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:shader>/assets)
