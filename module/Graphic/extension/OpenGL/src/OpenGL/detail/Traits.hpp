#ifndef GAMEENGINEBONGO_OPENGL_TRAITS_HPP
#define GAMEENGINEBONGO_OPENGL_TRAITS_HPP

#include <OpenGL/BufferObject.hpp>

namespace OpenGL::detail
{
    template<typename T, typename ContainerT>
    struct container_trait
    {
        using container_type = ContainerT;

        static void copy(const container_type &scontainer,
                         std::size_t           soffset,
                         container_type       &dcontainer,
                         std::size_t           doffset,
                         std::size_t           count)
        {
            OpenGL::memcpy(scontainer, soffset * sizeof(T), dcontainer, doffset * sizeof(T), count * sizeof(T));
        }

        static void write(container_type &container, std::size_t offset, const T *data, std::size_t count)
        {
            OpenGL::write(container, offset * sizeof(T), data, count * sizeof(T));
        }

        static ContainerT create(std::size_t len) { return ContainerT::create(len * sizeof(T)); }

        template<std::size_t len>
        static ContainerT create(const T (&data)[len])
        {
            return ContainerT::create(data);
        }
    };
}// namespace OpenGL::detail

#endif// GAMEENGINEBONGO_OPENGL_TRAITS_HPP
