#ifndef BONGOMOUNTAIN_VERTEXBUFFERPRIVATE_HPP
#define BONGOMOUNTAIN_VERTEXBUFFERPRIVATE_HPP

#include <cstdint>
#include <memory>
#include <core/functional.hpp>

namespace render::detail
{
    class VertexBufferPrivate
    {
    public:
        virtual ~VertexBufferPrivate() = default;

        virtual void        create(std::size_t size)                                          = 0;
        virtual void        create(const void *data, std::size_t size)                        = 0;
        virtual void        overwrite(std::size_t offset, const void *data, std::size_t size) = 0;
        virtual void        destroy()                                                         = 0;
        virtual void       *map()                                                             = 0;
        virtual const void *map() const                                                       = 0;
        virtual void        unmap(const void *) const                                         = 0;
        virtual void        bind() const                                                      = 0;
    };
}// namespace render::detail

#endif// BONGOMOUNTAIN_VERTEXBUFFERPRIVATE_HPP
