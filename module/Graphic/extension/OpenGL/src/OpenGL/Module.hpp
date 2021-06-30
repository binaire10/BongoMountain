#ifndef GAMEENGINEBONGO_OPENGL_MODULE_HPP
#define GAMEENGINEBONGO_OPENGL_MODULE_HPP

#include <string_view>
#include <Graphic/Context.hpp>

namespace OpenGL
{
    struct Module
    {
        static void           load(Graphic::GlobalContext &);
        static constexpr void unload(Graphic::GlobalContext &) {}
        static constexpr void beforeFrame(Graphic::GlobalContext &) {}
        static constexpr void afterFrame(Graphic::GlobalContext &) {}
    };
}// namespace OpenGL

#endif// GAMEENGINEBONGO_OPENGL_MODULE_HPP
