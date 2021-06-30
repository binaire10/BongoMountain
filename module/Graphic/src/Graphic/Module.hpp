#ifndef GAMEENGINEBONGO_MODULE_HPP
#define GAMEENGINEBONGO_MODULE_HPP

#include "config.hpp"

#include <string_view>
#include <memory>

#ifdef BM_PLATFORM_OPENGL
#    include <OpenGL/config.hpp>
#    include <OpenGL/Module.hpp>
#endif
#ifdef BM_PLATFORM_GLFW
#    include <GLFW/Module.hpp>
#endif

#include <Graphic/GraphicModule.hpp>

using GraphicModule = Graphic::ModularModule<
#ifdef BM_PLATFORM_GLFW
    glfw::Module
#    ifdef BM_PLATFORM_OPENGL
    ,
#    endif
#endif
#ifdef BM_PLATFORM_OPENGL
    OpenGL::Module
#endif
    >;


namespace Graphic
{
    BM_GRAPHIC_DCL std::unique_ptr<GraphicModule> createModule();
    BM_GRAPHIC_DCL RenderSurface *getMainSurface(std::unique_ptr<GraphicModule> &module);
}// namespace Graphic

#endif// GAMEENGINEBONGO_MODULE_HPP
