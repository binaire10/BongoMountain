project(triangle CXX)
add_executable(triangle main.cpp)
target_link_libraries(triangle PRIVATE BongoGraphic)

configure_binary_output(triangle)