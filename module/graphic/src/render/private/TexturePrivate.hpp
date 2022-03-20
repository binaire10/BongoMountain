#ifndef BONGOMOUNTAIN_TEXTUREPRIVATE_HPP
#define BONGOMOUNTAIN_TEXTUREPRIVATE_HPP

#include <graphic/Image.hpp>

namespace render::detail
{
    class TexturePrivate
    {
    public:
        virtual ~TexturePrivate() = default;

        virtual void create(const graphic::ImageView &image)    = 0;
        virtual void bind() const                               = 0;
        virtual void bind(unsigned unit) const                  = 0;
        virtual void overwrite(const graphic::ImageView &image) = 0;
    };
}// namespace render::detail

#endif// BONGOMOUNTAIN_TEXTUREPRIVATE_HPP
