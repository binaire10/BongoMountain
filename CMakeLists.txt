cmake_minimum_required(VERSION 3.16)
project(BongoMountain CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(imgui CONFIG)
find_package(glfw3 CONFIG)
find_package(glad CONFIG)
find_package(glm CONFIG)
find_package(spdlog CONFIG)
find_package(easy_profiler CONFIG)
find_package(nlohmann_json CONFIG)
find_package(stb CONFIG)
find_package(OpenSSL COMPONENTS Crypto CONFIG)


function(configure_binary_output target)
    set_target_properties(${target}
            PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
            )
endfunction()

add_executable(main main.cpp)
configure_binary_output(main)

function(configure_plugin_output target)
    add_dependencies(main ${target})
    set_target_properties(${target}
            PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/extension"
            )
endfunction()

add_subdirectory(module)
add_subdirectory(extension)

target_link_libraries(main PRIVATE Graphic)