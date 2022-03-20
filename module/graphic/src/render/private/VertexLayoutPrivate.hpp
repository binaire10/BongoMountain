#ifndef BONGOMOUNTAIN_VERTEXLAYOUTPRIVATE_HPP
#define BONGOMOUNTAIN_VERTEXLAYOUTPRIVATE_HPP

#include "../VertexLayoutDescriptor.h"

namespace render::detail
{
    class VertexLayoutPrivate
    {
    public:
        virtual ~VertexLayoutPrivate() = default;

        virtual void create(const render::AttributeLayout *attrs, std::size_t count, std::size_t stride) = 0;
        virtual void bind() const                                                                        = 0;
        virtual void destroy()                                                                           = 0;
    };
}// namespace render::detail

#endif// BONGOMOUNTAIN_VERTEXLAYOUTPRIVATE_HPP
