project(triangle CXX)
add_executable(triangle src/main.cpp)
target_link_libraries(triangle PRIVATE Core Graphic extensions)

configure_binary_output(triangle)