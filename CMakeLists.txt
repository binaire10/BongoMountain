cmake_minimum_required(VERSION 3.20)
project(GameEngineBongo)

add_subdirectory(vendor)
add_subdirectory(module)

add_library(common_options INTERFACE)
target_compile_features(common_options INTERFACE cxx_std_20)

add_executable(GameEngineBongo main.cpp)
target_link_libraries(GameEngineBongo PUBLIC common_options)