project(triangle CXX)
add_executable(triangle src/main.cpp)
target_link_libraries(triangle PRIVATE BongoGraphic)

configure_binary_output(triangle)