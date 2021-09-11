project(GLAD_EXTENSION)
function(use_glad target)
    target_compile_features(${target} PUBLIC cxx_std_20)
    target_link_libraries(${target} PUBLIC BongoCore glad glm::glm common_options)
    target_compile_definitions(${target} PUBLIC BM_PLATFORM_OPENGL BM_PLATFORM_GLAD)
    target_include_directories(${target} PUBLIC ${GLAD_EXTENSION_SOURCE_DIR}/src)
    target_sources(${target} PRIVATE
            ${GLAD_EXTENSION_SOURCE_DIR}/src/OpenGL/Shader.cpp)
endfunction()
