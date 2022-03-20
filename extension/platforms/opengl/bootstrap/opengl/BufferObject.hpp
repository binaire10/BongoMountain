#ifndef BONGOMOUNTAIN_OPENGL_BUFFEROBJECT_HPP
#define BONGOMOUNTAIN_OPENGL_BUFFEROBJECT_HPP

#include <glad/glad.h>
#include <vector>
#include <memory>
#include <istream>

#include <core/resource.hpp>
#include <core/type_traits.hpp>

namespace opengl::detail
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
        template<typename ResourceHandleT>
        UnmapBufferFree(const ResourceHandleT &handle)
            : handle(handle.getResource())
        {}
        static_assert(std::is_trivially_destructible_v<value_type>);
        void operator()(value_type *)
        {
            glBindBuffer(target, handle);
            glUnmapBuffer(target);
        }

    private:
        GLuint handle;
    };

    template<typename T, GLenum target>
    using unique_buffer_ptr = std::unique_ptr<T[], UnmapBufferFree<T, target>>;
}// namespace opengl::detail

namespace opengl
{
    template<template<typename, typename> typename ResourceOwnerT, GLenum target>
    class BufferObjectGeneric;

    template<typename T, GLenum target>
    struct array_trait
    {
        using array_type = detail::unique_buffer_ptr<T, target>;

        using const_array_type = detail::unique_buffer_ptr<std::add_const_t<T>, target>;

        template<typename ResourceHandleT>
        static std::vector<T> dump(ResourceHandleT &handle, std::size_t length)
        {
            array_type data(reinterpret_cast<T *>(glMapBuffer(target, GL_READ_ONLY)),
                            detail::UnmapBufferFree<T, target>{ handle });
            return std::vector<T>(data.get(), data.get() + length);
        }
    };

    template<typename T,
             template<typename, typename>
             typename ResourceOwnerT,
             GLenum target,
             typename ArrayTraitT = array_trait<T, target>>
    class BufferVectorObjectGeneric
    {
    public:
        using resource_handler_type = ResourceOwnerT<GLuint, detail::BufferObjectTrait>;

        BufferVectorObjectGeneric()                                  = default;
        BufferVectorObjectGeneric(const BufferVectorObjectGeneric &) = delete;
        BufferVectorObjectGeneric &operator=(const BufferVectorObjectGeneric &) = delete;

        BufferVectorObjectGeneric(BufferVectorObjectGeneric &&move)
            : m_handle{ move.release() }
        {}

        BufferVectorObjectGeneric &operator=(BufferVectorObjectGeneric &&move) { m_handle = move.release(); }

        operator BufferObjectGeneric<core::resource_view, target>()
        {
            return BufferObjectGeneric<core::resource_view, target>{ m_handle };
        }

        void destroy() noexcept
        {
            m_handle.destroy();
            m_capacity = m_size = 0;
        }

        void bind() const noexcept { glBindBuffer(target, m_handle.getResource()); }

        template<std::size_t len>
        void create(const T (&data)[len])
        {
            create(data, len);
        }

        void create(const T *data, std::size_t len)
        {
            if(m_handle)
                m_handle.destroy();
            m_capacity = m_size = len;
            m_handle.create();
            bind();
            glBufferData(target, len * sizeof(T), data, GL_STATIC_DRAW);
        }

        void create(std::size_t len)
        {
            if(m_handle)
                m_handle.destroy();
            m_capacity = len;
            m_handle.create();
            bind();
            glBufferData(target, len * sizeof(T), nullptr, GL_DYNAMIC_DRAW);
        }

        template<std::size_t len>
        void insert(const T (&data)[len])
        {
            insert(data, len);
        }

        void insert(const T *data, std::size_t len)
        {
            reserve(len + m_size);
            glBufferSubData(target, m_size * sizeof(T), len * sizeof(T), data);
            m_size += len;
        }

        void overwrite(std::size_t offset, const T *data, std::size_t len)
        {
            glBufferSubData(target, offset * sizeof(T), len * sizeof(T), data);
        }

        void reserve(std::size_t capacity)
        {
            if(capacity > m_capacity)
            {
                auto hold         = ArrayTraitT::dump(m_handle, m_size);
                auto new_capacity = std::max(m_size * 2, capacity);
                create(new_capacity);
                glBufferSubData(target, 0, m_size * sizeof(T), std::data(hold));
                m_capacity = new_capacity;
            }
        }

        std::vector<T> dump() const { return ArrayTraitT::dump(m_handle, m_size); }

        void shrink_to_fit()
        {
            auto hold = ArrayTraitT::dump(m_handle, m_size);
            m_handle.destroy();

            m_capacity = m_size;
            m_handle.create();
            bind();
            glBufferData(target, m_size * sizeof(T), std::data(hold), GL_DYNAMIC_DRAW);
        }

        void pop_back()
        {
            if(m_size > 0)
                --m_size;
        }

        void load(std::istream &&loader) { load(loader); }

        void load(std::istream &loader)
        {
            std::vector<uint8_t>   data;
            std::istream::int_type value;
            while((value = loader.get()) != std::istream::traits_type::eof()) data.push_back(loader.get());
        }

        void clear() { m_size = 0; }

        [[nodiscard]] std::size_t size() const { return m_size; }

        [[nodiscard]] std::size_t capacity() const { return m_capacity; }

        const resource_handler_type &getHandle() const { return m_handle; }

        resource_handler_type release()
        {
            m_capacity = m_size = 0;
            return std::move(m_handle);
        }

    private:
        resource_handler_type m_handle;
        std::size_t           m_size     = 0;
        std::size_t           m_capacity = 0;


        template<template<typename, typename> typename, GLenum>
        friend class BufferObjectGeneric;
    };

    template<template<typename, typename> typename ResourceOwnerT, GLenum target>
    class BufferObjectGeneric
    {
    public:
        template<typename T>
        using array_type = detail::unique_buffer_ptr<T, target>;

        template<typename T>
        using const_array_type = detail::unique_buffer_ptr<std::add_const_t<T>, target>;

        using resource_handle_type = ResourceOwnerT<GLuint, detail::BufferObjectTrait>;

        constexpr BufferObjectGeneric() = default;

        operator BufferObjectGeneric<core::resource_view, target>()
        {
            return BufferObjectGeneric<core::resource_view, target>{ m_handle };
        }

        explicit BufferObjectGeneric(const resource_handle_type &mHandle)
            : m_handle(mHandle)
        {}

        template<typename T, typename ArrayTypeT>
        BufferObjectGeneric(const BufferVectorObjectGeneric<T, ResourceOwnerT, target, ArrayTypeT> &buffer)
            : BufferObjectGeneric(buffer.getHandle())
        {}

        template<typename T, typename ArrayTypeT>
        BufferObjectGeneric &operator=(const BufferVectorObjectGeneric<T, ResourceOwnerT, target, ArrayTypeT> &buffer)
        {
            m_handle = buffer.getHandle();
            return *this;
        }

        template<typename T, typename ArrayTypeT>
        explicit BufferObjectGeneric(BufferVectorObjectGeneric<T, ResourceOwnerT, target, ArrayTypeT> &&buffer)
            : BufferObjectGeneric(buffer.release())
        {}

        template<typename T, typename ArrayTypeT>
        BufferObjectGeneric &operator=(BufferVectorObjectGeneric<T, ResourceOwnerT, target, ArrayTypeT> &&buffer)
        {
            m_handle = buffer.release();
            return *this;
        }

        explicit BufferObjectGeneric(resource_handle_type &&mHandle)
            : m_handle(std::move(mHandle))
        {}

        void overwrite(std::size_t offset, const void *data, std::size_t size)
        {
            glBufferSubData(target, offset, size, data);
        }

        void destroy() noexcept { m_handle.destroy(); }

        void bind() const noexcept { glBindBuffer(target, m_handle.getResource()); }

        static BufferObjectGeneric create(std::size_t len)
        {
            BufferObjectGeneric buffer;

            buffer.m_handle.create();
            buffer.bind();
            glBufferData(target, len, nullptr, GL_DYNAMIC_DRAW);

            return buffer;
        }

        template<typename T>
        static BufferObjectGeneric create(const T *data, std::size_t len)
        {
            return create(static_cast<const void *>(data), len * sizeof(T));
        }

        static BufferObjectGeneric create(const void *data, std::size_t len)
        {
            BufferObjectGeneric buffer;

            buffer.m_handle.create();
            buffer.bind();
            glBufferData(target, len, data, GL_DYNAMIC_DRAW);

            return buffer;
        }

        template<typename T, std::size_t len>
        static BufferObjectGeneric create(const T (&data)[len])
        {
            BufferObjectGeneric buffer;
            buffer.m_handle.create();
            buffer.bind();
            glBufferData(target, len * sizeof(T), data, GL_STATIC_DRAW);
            return buffer;
        }

        [[nodiscard]] void *map() {
            return glMapBuffer(target, GL_READ_WRITE);
        }

        [[nodiscard]] const void *map() const {
            return glMapBuffer(target, GL_READ_ONLY);
        }

        void unmap(const void *) const {
            glUnmapBuffer(target);
        }

        const resource_handle_type &getHandle() const { return m_handle; }

        template<typename T>
        [[nodiscard]] const_array_type<T> getData() const noexcept
        {
            using result_type = std::add_const_t<T>;
            static_assert(std::is_trivially_constructible_v<result_type>);
            return detail::unique_buffer_ptr<result_type, target>(
                reinterpret_cast<result_type *>(glMapBuffer(target, GL_READ_ONLY)),
                detail::UnmapBufferFree<result_type, target>{ m_handle });
        }

        template<typename T>
        [[nodiscard]] array_type<T> getData() noexcept
        {
            static_assert(std::is_trivially_constructible_v<T>);
            return array_type<T>(
                reinterpret_cast<T *>(glMapBuffer(target, std::is_const_v<T> ? GL_READ_ONLY : GL_READ_WRITE)),
                detail::UnmapBufferFree<T, target>{ m_handle });
        }

    protected:
        resource_handle_type m_handle;
    };

    template<GLenum target>
    using BufferObjectView = BufferObjectGeneric<core::resource_view, target>;

    template<GLenum target>
    using BufferObject = BufferObjectGeneric<core::resource, target>;

    template<GLenum target>
    using SharedBufferObject = BufferObjectGeneric<core::shared_resource, target>;

    template<typename T, GLenum target>
    using VectorBufferObject = BufferVectorObjectGeneric<T, core::resource, target>;

    template<typename T, GLenum target>
    using SharedVectorBufferObject = BufferVectorObjectGeneric<T, core::shared_resource, target>;

    template<GLenum target>
    using SharedBufferObject = BufferObjectGeneric<core::shared_resource, target>;

    using VertexBufferView = BufferObjectView<GL_ARRAY_BUFFER>;
    using VertexBuffer     = BufferObject<GL_ARRAY_BUFFER>;
    template<typename T>
    using VectorVertexBuffer = VectorBufferObject<T, GL_ARRAY_BUFFER>;

    using IndexBufferView = BufferObjectView<GL_ELEMENT_ARRAY_BUFFER>;
    using IndexBuffer     = BufferObject<GL_ELEMENT_ARRAY_BUFFER>;
    template<typename T>
    using VectorIndexBuffer = VectorBufferObject<T, GL_ELEMENT_ARRAY_BUFFER>;
    template<typename T>
    using SharedVectorIndexBuffer = SharedVectorBufferObject<T, GL_ELEMENT_ARRAY_BUFFER>;

    template<typename T>
    using SharedVectorVertexBuffer = SharedVectorBufferObject<T, GL_ARRAY_BUFFER>;
    using SharedVertexBuffer       = SharedBufferObject<GL_ARRAY_BUFFER>;
    using SharedIndexBuffer        = SharedBufferObject<GL_ELEMENT_ARRAY_BUFFER>;
}// namespace opengl

#endif// BONGOMOUNTAIN_OPENGL_BUFFEROBJECT_HPP
