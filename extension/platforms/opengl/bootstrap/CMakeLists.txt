add_library(opengl_bootstrap STATIC)
target_sources(opengl_bootstrap PRIVATE
        opengl/BufferObject.hpp
        opengl/Shader.cpp
        opengl/Shader.hpp
        opengl/VertexLayout.hpp
        opengl/Texture.hpp)
target_include_directories(opengl_bootstrap PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(opengl_bootstrap PUBLIC glad::glad Graphic)
