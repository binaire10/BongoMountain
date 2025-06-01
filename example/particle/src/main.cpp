#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include <render/ParticleManager.hpp>
#include <fstream>
#include <core/Platform.hpp>
#include <core/Log.hpp>
#include <event/Event.hpp>
#include <event/EventDispatcher.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include "graphic/ImageLoader.hpp"
#include "graphic/Graphic.hpp"
#include "render/RenderDevice.hpp"
#include "event/application/WindowResizeEvent.h"
#include "event/application/WindowCloseEvent.h"

struct ColoredVertex
{
    glm::vec3 pos;
    glm::vec4 color;
};

class CustomLayer final : public core::Layer
{
public:
    static constexpr ColoredVertex array_data[]{ ColoredVertex{ { -0.5f, -0.5f, 0.0f }, { 0.8f, 0.2f, 0.8f, 1.0f } },
                                                 ColoredVertex{ { 0.5f, -0.5f, 0.0f }, { 0.2f, 0.3f, 0.8f, 1.0f } },
                                                 ColoredVertex{ { 0.0f, 0.5f, 0.0f }, { 0.8f, 0.8f, 0.2f, 1.0f } } };
    static constexpr render::RenderDeviceInfo deviceInfo{ 1600, 1200, render::opengl_t{},
                                                          render::window_t{ .name = "particle", .isVisible = true } };

    void onAttach() override
    {
        device = graphic::createRenderDevice(core::Platform::getInstance(), deviceInfo);
        if(!device)
            return;

        particleManager.initialize(100'000, device.get());
        glm::mat4 projection = glm::mat4(1.f);

        particleManager.configure(glm::ortho(-ratio, ratio, -1.f, 1.f), 0);
        start = std::chrono::high_resolution_clock::now();
        device->setClearColor(0.15, 0.15, 0.15, 1);

        std::mt19937_64                       rand(std::random_device{}());
        std::uniform_real_distribution<float> distribution{ 0, 120 };
        std::uniform_int_distribution<int>    distribution1{ 0, 1 };
        ratio = (float)deviceInfo.width / deviceInfo.height;

        for(unsigned i{}; i < 50; ++i)
            for(unsigned j{}; j < 50; ++j)
                particleManager.addParticle(
                    { (float(i) / 50.f - 0.5f), (float(j) / 50.f - 0.5f), 0 }, { (1.f / 50.f) * 2, (1.f / 50.f) * 2 },
                    { 0, 0, 0 }, glm::angleAxis(0.f, glm::vec3{ 0, 0, 1 }),
                    glm::angleAxis(glm::degrees(5.f * float(distribution1(rand) * 2 - 1)), glm::vec3{ 0, 0, 1 }),
                    distribution(rand), glm::vec4{ 1, 0, 0, 1 });
    }

    void onDetach() override
    {
        particleManager.destroy();
        device = nullptr;
    }

    void onBegin() override
    {
        device->make_current();
        device->clear();
    }
    void onEnd() override { device->flush(); }

    void onUpdate() override
    {
        particleManager.configure(
            glm::ortho(-ratio, ratio, -1.f, 1.f),
            std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock ::now() - start)
                .count());
        particleManager.draw();
    }

    void onEvent(Event &event) override
    {
        EventDispatcher dispatcher{ event };
        dispatcher.dispatch<WindowResizeEvent>([this](const WindowResizeEvent &event) {
            device->viewport(glm::vec2(event.getWidth(), event.getHeight()));
            ratio = (float)event.getWidth() / event.getHeight();
            core::Platform::getInstance().compute();
        });
        dispatcher.dispatch<WindowCloseEvent>([](auto &e) { core::Platform::getInstance().exit(); });
    }

private:
    std::unique_ptr<render::RenderDevice>          device;
    render::ParticleManager                        particleManager;
    std::chrono::high_resolution_clock::time_point start;
    float                                          ratio;
};

int main(int argc, const char **argv)
{
    core::Log      log;
    core::Platform platform{ argc, argv };
    platform.load(platform.getApplicationPath().parent_path().append("extension"));

    platform.addLayer(std::make_shared<CustomLayer>());

    platform.exec();
}