#ifndef GAMEENGINEBONGO_GLFW_RENDERSURFACE_HPP
#define GAMEENGINEBONGO_GLFW_RENDERSURFACE_HPP

#include "Graphic/config.hpp"
#include <string_view>
#include <Graphic/BufferLayout.h>
#include <glm/vec2.hpp>
#include <Graphic/RenderCommand.hpp>


namespace Graphic
{

    struct RenderSurfacePrivate;

    class BM_GRAPHIC_DCL RenderSurface
    {
    public:
        RenderSurface(std::string_view title, unsigned width, unsigned height);
        ~RenderSurface() noexcept;

        void       setWindowName(std::string_view title);
        void       show() noexcept;
        void       hide() noexcept;
        void       resize(unsigned width, unsigned height) noexcept;
        void       make_current() noexcept;
        void       swap() noexcept;
        glm::uvec2 getSize() noexcept;

    private:
        static RenderSurfacePrivate *create(std::string_view title, unsigned width, unsigned height) noexcept;
        RenderSurfacePrivate *       data;
    };
}// namespace Graphic

#endif// GAMEENGINEBONGO_GLFW_RENDERSURFACE_HPP
