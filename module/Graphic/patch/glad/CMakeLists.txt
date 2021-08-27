project(GLAD)

add_library(glad_d SHARED "")
target_include_directories(glad_d PUBLIC debug/include)
target_sources(glad_d PRIVATE debug/src debug/src/glad.c
        PUBLIC
        debug/include/glad/glad.h
        debug/include/KHR/khrplatform.h)
target_compile_definitions(glad_d PUBLIC GLAD_GLAPI_EXPORT PRIVATE GLAD_GLAPI_EXPORT_BUILD)
set_target_properties(glad_d PROPERTIES OUTPUT_NAME "gladd")

add_library(glad_r SHARED "")
target_compile_definitions(glad_r PUBLIC GLAD_GLAPI_EXPORT PRIVATE GLAD_GLAPI_EXPORT_BUILD)
target_include_directories(glad_r PUBLIC release/include)
target_sources(glad_r PRIVATE release/src release/src/glad.c
        PUBLIC
        release/include/glad/glad.h
        release/include/KHR/khrplatform.h)
set_target_properties(glad_r PROPERTIES OUTPUT_NAME "glad")


add_library(glad INTERFACE)
target_link_libraries(glad INTERFACE debug glad_d optimized glad_r)
