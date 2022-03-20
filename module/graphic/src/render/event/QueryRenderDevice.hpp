#ifndef BONGOMOUNTAIN_QUERYRENDERDEVICE_HPP
#define BONGOMOUNTAIN_QUERYRENDERDEVICE_HPP

#include <memory>
#include <nlohmann/json.hpp>

#include "../../graphic/config.hpp"
#include "../RenderDevice.hpp"
#include "event/Event.hpp"

namespace render::event
{
    class BM_GRAPHIC_DCL QueryRenderDevice : public Event
    {
    public:
        EVENT_CLASS(Layer, QueryRenderDevice)

        explicit QueryRenderDevice(const render::RenderDeviceInfo &info)
            : info(info)
        {}

        inline void setRenderDevice(std::unique_ptr<render::RenderDevice> &&target)
        {
            renderTarget = std::move(target);
        }

        [[nodiscard]] const std::unique_ptr<render::RenderDevice> &getRenderDevice() const { return renderTarget; }
        [[nodiscard]] std::unique_ptr<render::RenderDevice>       &getRenderDevice() { return renderTarget; }
        [[nodiscard]] inline const std::variant<render::RenderDeviceInfo, nlohmann::json> &getInfo() const noexcept
        {
            return info;
        }

    private:
        std::variant<render::RenderDeviceInfo, nlohmann::json> info;
        std::unique_ptr<render::RenderDevice>                  renderTarget;
    };
}// namespace render::event

#endif// BONGOMOUNTAIN_QUERYRENDERDEVICE_HPP
