cmake_minimum_required(VERSION 3.22)
project(BongoMountain CXX)
set(CMAKE_CXX_STANDARD 20)

if (NOT EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake OR ${CMAKE_SOURCE_DIR}/conanfile.py IS_NEWER_THAN ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    # Download automatically, you can also just copy the conan.cmake file
    if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
        message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
        file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake")
    endif ()

    include(${CMAKE_BINARY_DIR}/conan.cmake)

    conan_cmake_run(
            CONANFILE conanfile.py
            GENERATORS cmake_find_package
            BASIC_SETUP CMAKE_TARGETS BUILD missing NO_OUTPUT_DIRS)

    file(TOUCH_NOCREATE ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
else ()
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
endif ()
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

find_package(imgui CONFIG)
find_package(glfw3 CONFIG)
find_package(glad CONFIG)
find_package(glm CONFIG)
find_package(spdlog CONFIG)
find_package(easy_profiler CONFIG)
find_package(nlohmann_json CONFIG)
find_package(stb CONFIG)


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