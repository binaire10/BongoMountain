#ifndef GAMEENGINEBONGO_CONTEXT_HPP
#define GAMEENGINEBONGO_CONTEXT_HPP

#ifdef BM_PLATFORM_GLAD
#    include <OpenGL/config.hpp>
#endif
#ifdef BM_PLATFORM_GLFW
#    include <GLFW/Context.hpp>
#endif

namespace Graphic
{
    struct GlobalContext
#if defined(BM_PLATFORM_GLFW)
        :
#endif
#ifdef BM_PLATFORM_GLFW
        glfw::Context
#endif
    {};
}// namespace Graphic
#endif// GAMEENGINEBONGO_CONTEXT_HPP
