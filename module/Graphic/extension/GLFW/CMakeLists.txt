project(GLFW_EXTENSION)
function(use_glfw target)
    target_compile_definitions(${target} PUBLIC BM_PLATFORM_GLFW)
    target_link_libraries(${target} PUBLIC BongoCore common_options PRIVATE glfw)
    target_include_directories(${target} PUBLIC ${GLFW_EXTENSION_SOURCE_DIR}/src)
    target_sources(${target} PRIVATE
            ${GLFW_EXTENSION_SOURCE_DIR}/src/GLFW/GraphicApplication.cpp
            ${GLFW_EXTENSION_SOURCE_DIR}/src/GLFW/Module.cpp
            ${GLFW_EXTENSION_SOURCE_DIR}/src/GLFW/RenderSurface.cpp)
endfunction()
