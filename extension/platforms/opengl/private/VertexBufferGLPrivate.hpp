#ifndef BONGOMOUNTAIN_VERTEXBUFFERGLPRIVATE_HPP
#define BONGOMOUNTAIN_VERTEXBUFFERGLPRIVATE_HPP

#include <render/private/VertexBufferPrivate.hpp>
#include "opengl/BufferObject.hpp"

class VertexBufferGLPrivate final : public render::detail::VertexBufferPrivate
{
public:
    void create(std::size_t size) override;
    void create(const void *data, std::size_t size) override;
    void overwrite(std::size_t offset, const void *data, std::size_t size) override;
    void destroy() override;
    void bind() const override;

    [[nodiscard]] const void *map() const override;
    [[nodiscard]] void       *map() override;
    void                      unmap(const void *pVoid) const override;

private:
    opengl::VertexBuffer vertexBuffer;
};


#endif// BONGOMOUNTAIN_VERTEXBUFFERGLPRIVATE_HPP
