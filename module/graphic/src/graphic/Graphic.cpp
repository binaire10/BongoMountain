#include "Graphic.hpp"
#include <core/Platform.hpp>
#include <render/RenderDeviceFactory.hpp>

std::unique_ptr<render::RenderDevice> graphic::createRenderDevice(core::Platform                 &platform,
                                                                  const render::RenderDeviceInfo &info)
{
    return platform.getSharedInstance<render::RenderDeviceFactory>()->createRenderDevice(info);
}
