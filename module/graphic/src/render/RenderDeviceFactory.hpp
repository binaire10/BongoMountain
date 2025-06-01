#ifndef RENDERDEVICEFACTORY_HPP
#define RENDERDEVICEFACTORY_HPP

#include <memory>

namespace render {
    class RenderDevice;
    class RenderDeviceInfo;
    class RenderDeviceFactory
    {
    public:
        virtual ~RenderDeviceFactory() = default;

        virtual std::unique_ptr<RenderDevice> createRenderDevice(const RenderDeviceInfo &info) = 0;
    };
}

#endif// RENDERDEVICEFACTORY_HPP
