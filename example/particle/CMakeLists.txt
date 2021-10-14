project(particle CXX)
add_executable(particle src/main.cpp)
target_link_libraries(particle PRIVATE BongoRender)

configure_binary_output(particle)

add_custom_command(TARGET particle POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:particle>/assets)
