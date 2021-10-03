project(texture CXX)
add_executable(texture src/main.cpp)
target_link_libraries(texture PRIVATE BongoGraphic)

configure_binary_output(texture)

add_custom_command(TARGET texture POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:texture>/assets)
