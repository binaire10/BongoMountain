#ifndef BONGOMOUNTAIN_TEXTUREGLPRIVATE_HPP
#define BONGOMOUNTAIN_TEXTUREGLPRIVATE_HPP

#include <render/private/TexturePrivate.hpp>
#include "opengl/Texture.hpp"


class TextureGLPrivate final : public render::detail::TexturePrivate
{
public:
    void create(const graphic::ImageView &image) override;
    void bind() const override;
    void bind(unsigned id) const override;
    void overwrite(const graphic::ImageView &image) override;

private:
    opengl::Texture texture;
};


#endif// BONGOMOUNTAIN_TEXTUREGLPRIVATE_HPP
