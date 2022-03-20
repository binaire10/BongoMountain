#ifndef BONGOMOUNTAIN_INDEXBUFFEROBJECTGLPRIVATE_HPP
#define BONGOMOUNTAIN_INDEXBUFFEROBJECTGLPRIVATE_HPP

#include <render/private/IndexBufferObjectPrivate.hpp>
#include "opengl/BufferObject.hpp"

class IndexBufferObjectGLPrivate final : public render::detail::IndexBufferObjectPrivate
{
public:
    void create(std::size_t size) override;
    void create(const void *data, std::size_t size) override;
    void bind() const override;
    void destroy() override;

private:
    opengl::IndexBuffer m_handle;
};


#endif// BONGOMOUNTAIN_INDEXBUFFEROBJECTGLPRIVATE_HPP
