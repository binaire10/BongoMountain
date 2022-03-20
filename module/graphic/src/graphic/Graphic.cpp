#include "Graphic.hpp"
#include "../render/event/QueryRenderDevice.hpp"

std::unique_ptr<render::RenderDevice> graphic::createRenderDevice(core::Platform &platform, const render::RenderDeviceInfo &info)
{
    render::event::QueryRenderDevice query{ info };
    platform.dispatchEvent(query);
    return std::move(query.getRenderDevice());
}
