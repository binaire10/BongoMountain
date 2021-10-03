#ifndef GAMEENGINEBONGO_OPENGL_BUFFEROBJECT_HPP
#define GAMEENGINEBONGO_OPENGL_BUFFEROBJECT_HPP

#include <Core/ressource.hpp>
#include <Core/type_traits.hpp>
#include <memory>

#include "config.hpp"

namespace OpenGL
{
    namespace detail
    {
        struct BufferObjectTrait
        {
            static constexpr GLuint invalid_resource() noexcept { return 0; }

            static inline GLuint create() noexcept
            {
                GLuint id;
                glGenBuffers(1, &id);
                return id;
            }

            static inline void destroy(const GLuint &id) noexcept { glDeleteBuffers(1, &id); }
        };

        template<typename T, GLenum target>
        class UnmapBufferFree
        {
        public:
            using value_type = T;
            UnmapBufferFree(const Core::shared_resource<GLuint, BufferObjectTrait> &handle)
                : handle(handle)
            {}
            static_assert(std::is_trivially_destructible_v<value_type>);
            void operator()(value_type *)
            {
                glBindBuffer(target, handle.getResource());
                glUnmapBuffer(target);
            }

        private:
            Core::shared_resource<GLuint, BufferObjectTrait> handle;
        };

        template<typename T, GLenum target>
        using unique_buffer_ptr = std::unique_ptr<T[], UnmapBufferFree<T, target>>;
    }// namespace detail


    template<template<typename, typename> typename ResourceOwnerT, GLenum target>
    class BufferObjectGeneric
    {
    public:
        template<typename T>
        using array_type = detail::unique_buffer_ptr<T, target>;

        void destroy() noexcept { handle.destroy(); }

        void bind() const noexcept { glBindBuffer(target, handle.getResource()); }

        template<typename T = uint8_t>
        static BufferObjectGeneric create(std::size_t len)
        {
            BufferObjectGeneric buffer;

            buffer.handle.create();
            buffer.bind();
            glBufferData(target, len * sizeof(T), nullptr, GL_DYNAMIC_DRAW);

            return buffer;
        }

        template<typename T, std::size_t len>
        static BufferObjectGeneric create(const T (&data)[len])
        {
            BufferObjectGeneric buffer;
            buffer.handle.create();
            buffer.bind();
            glBufferData(target, len * sizeof(T), data, GL_STATIC_DRAW);
            return buffer;
        }

        const ResourceOwnerT<GLuint, detail::BufferObjectTrait> &getHandle() const { return handle; }

        template<typename T>
        [[nodiscard]] detail::unique_buffer_ptr<std::add_const_t<T>, target> getData() const noexcept
        {
            using result_type = std::add_const_t<T>;
            static_assert(std::is_trivially_constructible_v<result_type>);
            return detail::unique_buffer_ptr<result_type, target>(
                reinterpret_cast<result_type *>(glMapBuffer(target, GL_READ_ONLY)),
                detail::UnmapBufferFree<result_type, target>{ handle });
        }

        template<typename T>
        [[nodiscard]] array_type<T> getData() noexcept
        {
            static_assert(std::is_trivially_constructible_v<T>);
            return array_type<T>(
                reinterpret_cast<T *>(glMapBuffer(target, std::is_const_v<T> ? GL_READ_ONLY : GL_READ_WRITE)),
                detail::UnmapBufferFree<T, target>{ handle });
        }

    protected:
        ResourceOwnerT<GLuint, detail::BufferObjectTrait> handle;
    };


    template<GLenum target>
    using BufferObject = BufferObjectGeneric<Core::resource, target>;

    template<GLenum target>
    using SharedBufferObject = BufferObjectGeneric<Core::shared_resource, target>;

    using VertexBuffer = BufferObject<GL_ARRAY_BUFFER>;
    using IndexBuffer  = BufferObject<GL_ELEMENT_ARRAY_BUFFER>;

    using SharedVertexBuffer = SharedBufferObject<GL_ARRAY_BUFFER>;
    using SharedIndexBuffer  = SharedBufferObject<GL_ELEMENT_ARRAY_BUFFER>;

    template<template<typename, typename> typename AOwner, template<typename, typename> typename BOwner, GLenum target>
    void memcpy(const BufferObjectGeneric<AOwner, target> &src,
                std::size_t                                soffset,
                BufferObjectGeneric<BOwner, target>       &dest,
                std::size_t                                doffset,
                std::size_t                                size)
    {
        static_assert(
            Core::is_same_template_v<AOwner, Core::resource> or Core::is_same_template_v<AOwner, Core::shared_resource>,
            "invalid resource manager of source");
        static_assert(
            Core::is_same_template_v<BOwner, Core::resource> or Core::is_same_template_v<BOwner, Core::shared_resource>,
            "invalid resource manager of destination");
        glCopyBufferSubData(src.getHandle().getResource(), dest.getHandle().getResource(), soffset, doffset, size);
    }

    template<template<typename, typename> typename Owner, GLenum target>
    void write(BufferObjectGeneric<Owner, target> &dest, std::size_t doffset, const void *data, std::size_t size)
    {
        dest.bind();
        glBufferSubData(target, doffset, size, data);
    }
}// namespace OpenGL

#endif// GAMEENGINEBONGO_OPENGL_BUFFEROBJECT_HPP
