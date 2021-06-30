#ifndef GAMEENGINEBONGO_RENDERTYPE_HPP
#define GAMEENGINEBONGO_RENDERTYPE_HPP

namespace Graphic
{
    enum class RenderType {
#ifdef BM_PLATFORM_OPENGL
        OpenGL = 1,
#endif
    };
}// namespace Graphic

#endif// GAMEENGINEBONGO_RENDERTYPE_HPP
