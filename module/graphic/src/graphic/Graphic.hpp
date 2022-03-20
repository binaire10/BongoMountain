#ifndef BONGOMOUNTAIN_GRAPHIC_HPP
#define BONGOMOUNTAIN_GRAPHIC_HPP

#include <memory>
#include "config.hpp"
#include <core/Platform.hpp>
#include "../render/RenderDevice.hpp"

namespace graphic
{
    BM_GRAPHIC_DCL std::unique_ptr<render::RenderDevice> createRenderDevice(core::Platform &platform, const render::RenderDeviceInfo &info);
}

#endif// BONGOMOUNTAIN_GRAPHIC_HPP
