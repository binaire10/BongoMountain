#ifndef GAMEENGINEBONGO_OPENGL_CONTEXT_HPP
#define GAMEENGINEBONGO_OPENGL_CONTEXT_HPP

#include <optional>
#include "RenderSurface.hpp"

struct GLFWwindow;
namespace glfw
{
    struct Context
    {
        std::optional<Graphic::RenderSurface> mainSurface;
    };
}// namespace glfw

#endif// GAMEENGINEBONGO_OPENGL_CONTEXT_HPP
