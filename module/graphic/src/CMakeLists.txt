add_library(Graphic SHARED)
target_sources(Graphic
        PRIVATE
        graphic/config.hpp
        graphic/Graphic.hpp
        graphic/Graphic.cpp
        graphic/TextureFormat.hpp
        render/Texture.hpp
        render/Texture.cpp
        graphic/Image.hpp
        graphic/Image.cpp
        graphic/event/ReadImage.hpp
        graphic/event/ReadImage.cpp
        render/event/QueryRenderDevice.cpp
        render/event/QueryRenderDevice.hpp
        render/RenderElement.hpp
        render/Shader.hpp
        render/Shader.cpp
        render/IndexBufferObject.hpp
        render/IndexBufferObject.cpp
        render/RenderDevice.hpp
        render/VertexBufferObject.hpp
        render/VertexBufferObject.cpp
        render/VertexLayout.cpp
        render/VertexLayout.hpp
        render/VertexLayoutDescriptor.h
        render/private/IndexBufferObjectPrivate.hpp
        render/private/ShaderPrivate.hpp
        render/private/TexturePrivate.hpp
        render/private/VertexBufferPrivate.hpp
        render/private/VertexLayoutPrivate.hpp
        )
target_link_libraries(Graphic PUBLIC Core)
target_include_directories(Graphic PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_compile_definitions(Graphic PUBLIC BM_SHARED_GRAPHIC PRIVATE BM_BUILD_GRAPHIC)
configure_binary_output(Graphic)