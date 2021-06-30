find_package(nlohmann_json 3.2.0 QUIET)
if (NOT nlohmann_json_FOUND)
    set(NLOHMANN_JSON_OUTPUT ${CMAKE_SOURCE_DIR}/dependencies CACHE INTERNAL "Output directory of download")
    set(NLOHMANN_JSON_DIR nlomannJson CACHE INTERNAL "SRC directory name of benchmark")
    set(NLOHMANN_JSON_PATH ${NLOHMANN_JSON_OUTPUT}/${NLOHMANN_JSON_DIR} CACHE INTERNAL "Full path name to benchmark directory")

    if (NOT EXISTS ${NLOHMANN_JSON_PATH}/nlohmann/json.hpp)
        message(STATUS "download json header only to ${NLOHMANN_JSON_OUTPUT}")
        file(MAKE_DIRECTORY ${NLOHMANN_JSON_PATH}/nlohmann)
        file(DOWNLOAD https://github.com/nlohmann/json/releases/download/v3.9.1/json.hpp ${NLOHMANN_JSON_PATH}/nlohmann/json.hpp)

    endif ()
    message(STATUS "use downloaded json header only")

    add_library(nlohmann_json INTERFACE "")
    target_include_directories(nlohmann_json INTERFACE ${NLOHMANN_JSON_PATH})

    add_library(nlohmann_json::nlohmann_json ALIAS nlohmann_json)
    #    add_subdirectory(${BENCHMARK_PATH})
endif ()
