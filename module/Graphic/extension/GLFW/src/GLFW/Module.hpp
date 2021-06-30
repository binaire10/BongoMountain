#ifndef GAMEENGINEBONGO_GLFW_MODULE_HPP
#define GAMEENGINEBONGO_GLFW_MODULE_HPP

#include "Context.hpp"

namespace glfw
{

    struct Module
    {
        static void           load(Context &ctx);
        static void           unload(Context &ctx) noexcept;
        static constexpr void beforeFrame(Context &ctx) noexcept {}
        static constexpr void afterFrame(Context &ctx) noexcept {}
    };
}// namespace glfw

#endif// GAMEENGINEBONGO_GLFW_MODULE_HPP
