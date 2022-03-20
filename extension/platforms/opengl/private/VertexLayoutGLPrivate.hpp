#ifndef BONGOMOUNTAIN_VERTEXLAYOUTGLPRIVATE_HPP
#define BONGOMOUNTAIN_VERTEXLAYOUTGLPRIVATE_HPP

#include <render/private/VertexLayoutPrivate.hpp>
#include "opengl/VertexLayout.hpp"

class VertexLayoutGLPrivate final : public render::detail::VertexLayoutPrivate
{
public:
    void create(const render::AttributeLayout *attrs, std::size_t count, std::size_t stride) override;
    void bind() const override;
    void destroy() override;

private:
    opengl::BasicVertexLayout m_handle;
};


#endif// BONGOMOUNTAIN_VERTEXLAYOUTGLPRIVATE_HPP
