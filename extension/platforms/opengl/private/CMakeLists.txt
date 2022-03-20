add_library(private_opengl STATIC)
target_sources(private_opengl PRIVATE
        VertexLayoutGLPrivate.hpp
        ShaderGLPrivate.cpp
        IndexBufferObjectGLPrivate.cpp
        VertexLayoutGLPrivate.cpp
        IndexBufferObjectGLPrivate.hpp
        VertexBufferGLPrivate.hpp
        TextureGLPrivate.cpp
        VertexBufferGLPrivate.cpp
        ShaderGLPrivate.hpp
        TextureGLPrivate.hpp)
target_include_directories(private_opengl PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(private_opengl PUBLIC opengl_bootstrap)