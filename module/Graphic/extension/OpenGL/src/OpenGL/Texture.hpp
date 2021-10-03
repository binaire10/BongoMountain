#ifndef GAMEENGINEBONGO_OPENGL_TEXTURE_HPP
#define GAMEENGINEBONGO_OPENGL_TEXTURE_HPP

#include "config.hpp"
#include <Core/ressource.hpp>
#include <Graphic/detail/TextureFormat.hpp>
#include <Core/Assert.hpp>

namespace OpenGL
{
    struct texture_trait
    {
        static constexpr GLuint invalid_resource() noexcept { return 0; }

        static inline GLuint create() noexcept
        {
            GLuint id;
            glGenTextures(1, &id);
            return id;
        }

        static inline void destroy(const GLuint &id) noexcept { glDeleteTextures(1, &id); }
    };

    class Texture
    {
    public:
        Texture() = default;
        Texture(Texture &&) noexcept = default;
        Texture &operator=(Texture &&) noexcept = default;
        virtual ~Texture() = default;

        inline Texture(const void *data, uint16_t width, uint16_t height, Graphic::TextureFormat fmt) {
            using Graphic::TextureFormat;
            GLenum internalFormat = 0, dataFormat = 0;

            switch(fmt)
            {
            case TextureFormat::RGBA_8bit:
                internalFormat = GL_RGBA8;
                dataFormat = GL_RGBA;
                break;
            case TextureFormat::RGB_8bit:
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
                break;
            }


            BM_CORE_ASSERT(internalFormat && dataFormat, "Format not supported!");

            OpenGL::Texture texture;
            auto &id = texture.texture;
            id.create();
            glBindTexture(GL_TEXTURE_2D, id.getResource());

            glTexImage2D(GL_TEXTURE_2D, 0, dataFormat, width, height, 0, dataFormat,
                         GL_UNSIGNED_BYTE, data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }

        inline void bind(unsigned unit) const noexcept
        {
            glActiveTexture(GL_TEXTURE0 + unit);
            glBindTexture(GL_TEXTURE_2D, texture.getResource());
        }

        inline void bind() const noexcept
        {
            glBindTexture(GL_TEXTURE_2D, texture.getResource());
        }

        const Core::resource<GLuint, texture_trait> &getTexture() const { return texture; }

    private:
        Core::resource<GLuint, texture_trait> texture;
    };
}// namespace OpenGL

#endif// GAMEENGINEBONGO_OPENGL_TEXTURE_HPP
