#ifndef BONGOMOUNTAIN_OPENGL_TEXTURE_HPP
#define BONGOMOUNTAIN_OPENGL_TEXTURE_HPP

#include <glad/glad.h>
#include <core/resource.hpp>
#include <graphic/TextureFormat.hpp>
#include <graphic/Image.hpp>
#include <core/Assert.hpp>

namespace opengl
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

    template<template<typename, typename> typename ResourceOwnerT>
    class TextureGeneric
    {
    public:
        using ressource_type = ResourceOwnerT<GLuint, texture_trait>;

        TextureGeneric()                           = default;
        TextureGeneric(TextureGeneric &&) noexcept = default;
        TextureGeneric &operator=(TextureGeneric &&) noexcept = default;
        virtual ~TextureGeneric()                             = default;

        TextureGeneric(const void *data, uint16_t width, uint16_t height, graphic::TextureFormat fmt)
        {
            create(data, width, height, fmt);
        }

        explicit TextureGeneric(const graphic::ImageView &image) { create(image); }

        static TextureGeneric create(const graphic::ImageView &image)
        {
            return create(image.getData(), image.getWidth(), image.getWidth(), image.getFormat());
        }

        static TextureGeneric create(const void *data, uint16_t width, uint16_t height, graphic::TextureFormat fmt)
        {
            using graphic::TextureFormat;
            GLenum internalFormat = 0, dataFormat = 0;

            switch(fmt)
            {
            case TextureFormat::RGBA_8bit:
                internalFormat = GL_RGBA8;
                dataFormat     = GL_RGBA;
                break;
            case TextureFormat::RGB_8bit:
                internalFormat = GL_RGB8;
                dataFormat     = GL_RGB;
                break;
            }


            BM_CORE_ASSERT(internalFormat && dataFormat, "Format not supported!");

            TextureGeneric texture;
            auto          &id = texture.texture;
            id.create();
            glBindTexture(GL_TEXTURE_2D, id.getResource());

            glTexImage2D(GL_TEXTURE_2D, 0, dataFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            return texture;
        }

        inline void bind(unsigned unit) const noexcept
        {
            glActiveTexture(GL_TEXTURE0 + unit);
            glBindTexture(GL_TEXTURE_2D, texture.getResource());
        }

        inline void bind() const noexcept { glBindTexture(GL_TEXTURE_2D, texture.getResource()); }

        const ressource_type &getTexture() const { return texture; }

        void overwrite(const graphic::ImageView &image) {
            using graphic::TextureFormat;
            GLenum dataFormat = 0;

            switch(image.getFormat())
            {
            case TextureFormat::RGBA_8bit:
                dataFormat     = GL_RGBA;
                break;
            case TextureFormat::RGB_8bit:
                dataFormat     = GL_RGB;
                break;
            }


            BM_CORE_ASSERT(dataFormat, "Format not supported!");

            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.getWidth(), image.getHeight(), dataFormat, GL_UNSIGNED_BYTE, image.getData()); }

    private:
        ressource_type texture;
    };

    using TextureView   = TextureGeneric<core::resource_view>;
    using Texture       = TextureGeneric<core::resource>;
    using SharedTexture = TextureGeneric<core::resource>;
}// namespace opengl

#endif// BONGOMOUNTAIN_OPENGL_TEXTURE_HPP
