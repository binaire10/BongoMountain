#ifndef GAMEENGINEBONGO_GLFW_RENDERSURFACE_HPP
#define GAMEENGINEBONGO_GLFW_RENDERSURFACE_HPP

#include "Graphic/config.hpp"
#include <string_view>
#include <Graphic/BufferLayout.h>

#ifdef BM_PLATFORM_GLAD
#    include <OpenGL/BufferObject.hpp>
#    include <OpenGL/Shader.hpp>
#    include <OpenGL/VertexArray.hpp>
#    include <Core/PointerLikeStackVariable.hpp>
#    include <OpenGL/RenderCommand.hpp>
namespace Graphic
{
    using OpenGL::RenderCommand;
    using OpenGL::Shader;
}// namespace Graphic
#endif

namespace Graphic
{

    struct RenderSurfacePrivate;

    class BM_GRAPHIC_DCL RenderSurface
    {
    public:
#ifdef BM_PLATFORM_GLAD
        using RenderCommandStorage = PointerLikeStackVariable<RenderCommand>;
#endif

        RenderSurface(std::string_view title, unsigned width, unsigned height);
        ~RenderSurface() noexcept;

        void show() noexcept;
        void hide() noexcept;
        void resize(unsigned width, unsigned height) noexcept;
        void make_current() noexcept;
        void swap() noexcept;

        static constexpr RenderCommandStorage getRenderCommand() noexcept { return {}; }

    private:
        static RenderSurfacePrivate *create(std::string_view title, unsigned width, unsigned height) noexcept;
        RenderSurfacePrivate *       data;
    };
}// namespace Graphic

#endif// GAMEENGINEBONGO_GLFW_RENDERSURFACE_HPP
