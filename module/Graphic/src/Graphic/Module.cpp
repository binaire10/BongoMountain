#include "Module.hpp"

#ifdef BM_PLATFORM_GLFW
#    include <GLFW/Module.cpp>
#    include <GLFW/RenderSurface.cpp>
#    include <GLFW/GraphicApplication.cpp>
#endif
#ifdef BM_PLATFORM_GLAD
#    include <OpenGL/Module.cpp>
#endif


std::unique_ptr<GraphicModule> Graphic::createModule() { return std::make_unique<GraphicModule>(); }
Graphic::RenderSurface *       Graphic::getMainSurface(std::unique_ptr<GraphicModule> &module)
{
    return &*reinterpret_cast<GraphicModule *>(module.get())->getContext().mainSurface;
}
