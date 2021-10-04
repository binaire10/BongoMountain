cmake_minimum_required(VERSION 3.20)
project(GameEngineBongo)

if(CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL "binary dir and source dir cannot be the same")
endif()

include(cmake/AutoImport.cmake)

function(configure_binary_output target)
    set_target_properties(${target}
            PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
            )
endfunction()

add_library(common_options INTERFACE)
target_compile_features(common_options INTERFACE cxx_std_20)
set_target_properties(common_options
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )

add_subdirectory(module)
add_subdirectory(example)
add_subdirectory(test)

configure_binary_output(BongoCore)
configure_binary_output(BongoGraphic)
configure_binary_output(glfw)
configure_binary_output(glad_r)
configure_binary_output(glad_d)
configure_binary_output(spdlog)