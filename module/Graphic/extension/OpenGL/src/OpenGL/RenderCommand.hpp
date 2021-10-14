#ifndef GAMEENGINEBONGO_OPENGL_RENDERCOMMAND_HPP
#define GAMEENGINEBONGO_OPENGL_RENDERCOMMAND_HPP

#include <cstdint>
#include <glm/vec4.hpp>

#include "BufferObject.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "Texture.hpp"

#include <Graphic/BufferArray.hpp>

namespace OpenGL
{
    class RenderCommand
    {
    public:
        inline static void setViewport(unsigned x, unsigned y, unsigned width, unsigned height) noexcept
        {
            glViewport(static_cast<GLint>(x), static_cast<GLint>(y), static_cast<GLint>(width),
                       static_cast<GLint>(height));
        }

        inline static void
            drawIndexed(const Shader &shader, const VertexArray<void> &vertexArray, unsigned count = 0) noexcept
        {
            shader.bind();
            vertexArray.bind();
            vertexArray.getVertices().bind();
            glDrawArrays(GL_TRIANGLES, 0,
                         count ? static_cast<GLsizei>(count) : static_cast<GLsizei>(vertexArray.getCount()));
        }

        inline static void
            drawIndexed(const Shader &shader, const VertexArray<uint8_t> &vertexArray, unsigned count = 0) noexcept
        {
            shader.bind();
            vertexArray.bind();
            vertexArray.getIndices().bind();
            vertexArray.getVertices().bind();
            glDrawElements(GL_TRIANGLES, count ? count : static_cast<GLsizei>(vertexArray.getCount()), GL_UNSIGNED_BYTE,
                           nullptr);
        }

        inline static void
            drawIndexed(const Shader &shader, const VertexArray<uint16_t> &vertexArray, unsigned count = 0) noexcept
        {
            shader.bind();
            vertexArray.bind();
            vertexArray.getIndices().bind();
            vertexArray.getVertices().bind();
            glDrawElements(GL_TRIANGLES, count ? count : static_cast<GLsizei>(vertexArray.getCount()),
                           GL_UNSIGNED_SHORT, nullptr);
        }

        inline static void
            drawIndexed(const Shader &shader, const VertexArray<uint32_t> &vertexArray, unsigned count = 0) noexcept
        {
            shader.bind();
            vertexArray.bind();
            vertexArray.getIndices().bind();
            vertexArray.getVertices().bind();
            glDrawElements(GL_TRIANGLES, count ? count : static_cast<GLsizei>(vertexArray.getCount()), GL_UNSIGNED_INT,
                           nullptr);
        }

        inline static void drawTriangles(const Shader &shader, const VertexBuffer &vertex, unsigned count) noexcept
        {
            shader.bind();
            vertex.bind();
            glDrawArrays(GL_TRIANGLES, 0, count);
        }

        inline static void drawPoints(const Shader &shader, const BasicVertexArray &vertex, unsigned count = 0) noexcept
        {
            shader.bind();
            vertex.bind();
            vertex.getVertices().bind();
            glDrawArrays(GL_POINTS, 0, count ? count : vertex.getCount());
        }

        template<typename T>
        inline static void drawPoints(const Shader                              &shader,
                                      const Graphic::SharedVertexBufferArray<T> &vertex,
                                      unsigned                                   count = 0) noexcept
        {
            drawPoints(shader, static_cast<SharedVertexBuffer &>(vertex), count ? count : vertex.getCount());
        }

        inline static void drawPoints(const Shader &shader, const SharedVertexBuffer &vertex, unsigned count) noexcept
        {
            shader.bind();
            vertex.bind();
            glDrawArrays(GL_POINTS, 0, count);
        }

        inline static void drawPoints(const Shader &shader, const VertexBuffer &vertex, unsigned count) noexcept
        {
            shader.bind();
            vertex.bind();
            glDrawArrays(GL_POINTS, 0, count);
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

        inline static int getTextureUnitCount() noexcept
        {
            GLint result;
            glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &result);
            return result;
        }

        inline static int getMaxBufferSize() noexcept
        {
            GLint result;
            glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &result);
            return result;
        }
    };
}// namespace OpenGL

#endif// GAMEENGINEBONGO_OPENGL_RENDERCOMMAND_HPP
