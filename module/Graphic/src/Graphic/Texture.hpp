#ifndef GAMEENGINEBONGO_GRAPHIC_TEXTURE_HPP
#define GAMEENGINEBONGO_GRAPHIC_TEXTURE_HPP

#if defined(BM_PLATFORM_OPENGL)

#    include <OpenGL/RenderCommand.hpp>

#    include <OpenGL/BufferObject.hpp>
#    include <OpenGL/Shader.hpp>
#    include <OpenGL/VertexArray.hpp>
#    include <OpenGL/Texture.hpp>
#    include <Core/PointerLikeStackVariable.hpp>

namespace Graphic
{
    class BM_GRAPHIC_DCL Texture : public OpenGL::Texture
    {
    public:
        using OpenGL::Texture::Texture;

        static Texture        load_texture(std::string_view filename);
        static inline Texture load_texture(std::istream &&stream) { return load_texture(stream); }
        static Texture        load_texture(std::istream &stream);
        static Texture load_texture(const void *data, uint16_t width, uint16_t height, Graphic::TextureFormat fmt);
    };
}// namespace Graphic

#else
#    error "invalid render platform"
#endif

#endif// GAMEENGINEBONGO_GRAPHIC_TEXTURE_HPP
