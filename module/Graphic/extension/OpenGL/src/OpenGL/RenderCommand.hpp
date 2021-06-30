#ifndef GAMEENGINEBONGO_RENDERCOMMAND_HPP
#define GAMEENGINEBONGO_RENDERCOMMAND_HPP

#include <cstdint>
#include <glm/vec4.hpp>

namespace OpenGL
{
    class RenderCommand
    {
    public:
        inline static VertexBufferObject createBuffer(std::size_t n) noexcept
        {
            VertexBufferObject vbo;
            vbo.create(n);
            return vbo;
        }

        inline static VertexBufferObject createBuffer(void *data, std::size_t n) noexcept
        {
            VertexBufferObject vbo;
            vbo.create(data, n);
            return vbo;
        }

        template<typename T, std::size_t n>
        inline static VertexBufferObject createBuffer(T (&data)[n]) noexcept
        {
            return createBuffer(data, n * sizeof(T));
        }

        inline static IndexBufferObject createBufferIndex(std::size_t n) noexcept
        {
            IndexBufferObject ibo;
            ibo.create(n);
            return ibo;
        }

        template<typename T, std::size_t n>
        inline static IndexBufferStorageObject<T> createBufferIndexStorage(T (&data)[n]) noexcept
        {
            IndexBufferStorageObject<T> ibo;
            ibo.create(data, n);
            return ibo;
        }

        template<typename T>
        inline static IndexBufferStorageObject<T> createBufferIndexStorage(const T *data, std::size_t n) noexcept
        {
            IndexBufferStorageObject<T> ibo;
            ibo.create(data, n);
            return ibo;
        }

        inline static IndexBufferObject createBufferIndex(void *data, std::size_t n) noexcept
        {
            IndexBufferObject ibo;
            ibo.create(data, n);
            return ibo;
        }

        template<std::size_t n, typename T>
        inline static VertexArray createVertexArray(const BufferLayout<n> &     map,
                                                    IndexBufferStorageObject<T> indices,
                                                    VertexBufferObject          vertices) noexcept
        {
            VertexArray vertexArray;
            vertexArray.create(map, std::move(indices), std::move(vertices));
            return vertexArray;
        }

        template<typename T, std::size_t n>
        inline static IndexBufferObject createBufferIndex(T (&data)[n]) noexcept
        {
            return createBufferIndex(data, n * sizeof(T));
        }

        inline static Shader createShader(std::string_view src) { return Shader::createFromCode(src); }


        inline static void renderTriangle(const Shader &shader, const VertexArray &vertexArray) noexcept
        {
            shader.bind();
            vertexArray.bind();
            vertexArray.getIndices().bind();
            vertexArray.getVertices().bind();
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexArray.getCount()));
        }

        inline static void
            renderTriangle(const Shader &shader, const VertexBufferObject &vertex, unsigned count) noexcept
        {
            shader.bind();
            vertex.bind();
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(count));
        }

        inline static void setClearColor(float r, float g, float b, float a) noexcept { glClearColor(r, g, b, a); }

        inline static void setClearColor(const glm::vec4 &color) noexcept
        {
            glClearColor(color.r, color.g, color.b, color.a);
        }

        inline static void clear() noexcept
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
    };
}// namespace OpenGL

#endif// GAMEENGINEBONGO_RENDERCOMMAND_HPP
