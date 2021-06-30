#ifndef GAMEENGINEBONGO_BUFFEROBJECT_HPP
#define GAMEENGINEBONGO_BUFFEROBJECT_HPP

#include <Core/Ressource.hpp>
#include <Graphic/BufferCounter.hpp>

#include "config.hpp"

template<GLenum target>
class BufferObject
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

public:
    void destroy() noexcept { buffer.destroy(); }

    void bind() const noexcept { glBindBuffer(target, buffer.getResource()); }

    void create(const void *data, std::size_t n) noexcept
    {
        buffer.create();
        glBindBuffer(target, buffer.getResource());
        glBufferData(target, n, data, GL_STATIC_DRAW);
    }

    void create(std::size_t n) noexcept
    {
        buffer.create();
        glBindBuffer(target, buffer.getResource());
        glBufferData(target, n, nullptr, GL_DYNAMIC_DRAW);
    }

    void write(std::size_t offset, void *data, std::size_t size) noexcept
    {
        glBufferSubData(target, offset, size, data);
    }

protected:
    Resource<GLuint, BufferObjectTrait> buffer;
};

using VertexBufferObject = BufferObject<GL_ARRAY_BUFFER>;
using IndexBufferObject  = BufferObject<GL_ELEMENT_ARRAY_BUFFER>;

template<typename T>
using VertexBufferStorageObject = BufferCounter<T, BufferObject<GL_ARRAY_BUFFER>>;
template<typename T>
using IndexBufferStorageObject = BufferCounter<T, BufferObject<GL_ELEMENT_ARRAY_BUFFER>>;

#endif// GAMEENGINEBONGO_BUFFEROBJECT_HPP
