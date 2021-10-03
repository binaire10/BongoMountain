#include "Module.hpp"

std::unique_ptr<GraphicModule> Graphic::createModule() { return std::make_unique<GraphicModule>(); }
Graphic::RenderSurface *       Graphic::getMainSurface(std::unique_ptr<GraphicModule> &module)
{
    return &*reinterpret_cast<GraphicModule *>(module.get())->getContext().mainSurface;
}
