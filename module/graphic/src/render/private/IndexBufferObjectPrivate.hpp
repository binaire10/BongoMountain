#ifndef BONGOMOUNTAIN_INDEXBUFFEROBJECTPRIVATE_HPP
#define BONGOMOUNTAIN_INDEXBUFFEROBJECTPRIVATE_HPP

#include <cstdint>

namespace render::detail
{
    class IndexBufferObjectPrivate
    {
    public:
        virtual ~IndexBufferObjectPrivate() = default;

        virtual void create(std::size_t size) = 0;
        virtual void create(const void *data, std::size_t size) = 0;
        virtual void bind() const                               = 0;
        virtual void destroy()                                  = 0;
    };
}// namespace render::detail


#endif// BONGOMOUNTAIN_INDEXBUFFEROBJECTPRIVATE_HPP
