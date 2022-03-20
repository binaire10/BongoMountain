#ifndef BONGOMOUNTAIN_RENDERDEVICE_HPP
#define BONGOMOUNTAIN_RENDERDEVICE_HPP

#include <array>
#include <optional>
#include <variant>

#include <core/utility.hpp>

#include "IndexBufferObject.hpp"
#include "VertexBufferObject.hpp"
#include "Texture.hpp"
#include "VertexLayout.hpp"
#include "Shader.hpp"


namespace render
{
    template<std::size_t component>
    using version_t = std::array<unsigned, component>;

    struct opengl_t
    {
        std::optional<version_t<2>> version;
        bool                        isCoreProfile;
    };

    struct window_t
    {
        std::string_view name;
        bool             isVisible;
    };

    struct RenderDeviceInfo
    {
        int                     width, height;
        std::variant<opengl_t>  render;
        std::optional<window_t> window;
    };

    class RenderDevice
    {
    public:
        virtual ~RenderDevice()     = default;
        virtual void make_current() = 0;
        virtual void flush()        = 0;
        virtual void clear()        = 0;

        virtual void drawTriangles(const VertexBufferObject &vbo, const VertexLayout &vao, unsigned count) = 0;

        [[nodiscard]] virtual VertexBufferObject createVertexBuffer()                                   = 0;
        [[nodiscard]] virtual VertexBufferObject createVertexBuffer(std::size_t size)                   = 0;
        [[nodiscard]] virtual VertexBufferObject createVertexBuffer(const void *data, std::size_t size) = 0;

        template<typename T, std::size_t length>
        [[nodiscard]] inline VertexBufferObject createVertexBuffer(const T (&data)[length])
        {
            return createVertexBuffer(static_cast<const void *>(data), length * sizeof(T));
        }
        template<typename T>
        [[nodiscard]] inline VertexBufferObject createVertexBuffer(const T *data, std::size_t length)
        {
            return createVertexBuffer(static_cast<const void *>(data), length * sizeof(T));
        }

        [[nodiscard]] virtual Texture createTexture()                                = 0;
        [[nodiscard]] virtual Texture createTexture(const graphic::ImageView &image) = 0;

        [[nodiscard]] virtual VertexLayout createVertexLayout() = 0;
        [[nodiscard]] virtual VertexLayout
            createVertexLayout(const render::AttributeLayout *data, std::size_t count, std::size_t stride) = 0;

        template<typename VertexLayoutDescriptorT>
        [[nodiscard]] VertexLayout createVertexLayout(const VertexLayoutDescriptorT &descriptor)
        {
            return createVertexLayout(std::data(descriptor), std::size(descriptor), descriptor.stride);
        }

        [[nodiscard]] virtual Shader createShader()                              = 0;
        [[nodiscard]] virtual Shader createShaderFromCode(std::string_view code) = 0;
        [[nodiscard]] inline Shader  createShaderFromStream(std::istream &istream)
        {
            return createShaderFromCode(core::readAll(istream));
        }
        [[nodiscard]] inline Shader createShaderFromStream(std::ifstream &istream)
        {
            return createShaderFromCode(core::readAll(istream));
        }
        [[nodiscard]] inline Shader createShaderFromStream(std::istream &&istream)
        {
            return createShaderFromStream(istream);
        }
        [[nodiscard]] inline Shader createShaderFromStream(std::ifstream &&istream)
        {
            return createShaderFromStream(istream);
        }

        [[nodiscard]] virtual IndexBufferObject createIndexBuffer()                                   = 0;
        [[nodiscard]] virtual IndexBufferObject createIndexBuffer(std::size_t size)                   = 0;
        [[nodiscard]] virtual IndexBufferObject createIndexBuffer(const void *data, std::size_t size) = 0;

        template<typename T, std::size_t length>
        [[nodiscard]] inline IndexBufferObject createIndexBuffer(const T (&data)[length])
        {
            return createIndexBuffer(static_cast<const void *>(data), length * sizeof(T));
        }

        template<typename T>
        [[nodiscard]] inline IndexBufferObject createIndexBuffer(const T *data, std::size_t length)
        {
            return createIndexBuffer(static_cast<const void *>(data), length * sizeof(T));
        }

    protected:
    };

    class WindowRenderDevice : public RenderDevice
    {
    public:
        virtual void hide() = 0;
        virtual void show() = 0;
    };
}// namespace render

#endif// BONGOMOUNTAIN_RENDERDEVICE_HPP
