#include <Core/Log.hpp>
#include <Core/load_guard.hpp>
#include <Event/Application/WindowCloseEvent.h>
#include <Event/Application/WindowResizeEvent.h>
#include <Event/EventDispatcher.h>
#include <Graphic/BufferLayout.h>
#include <Graphic/GraphicApplication.hpp>
#include <Graphic/Module.hpp>
#include <Render/ParticleManager.hpp>
#include <chrono>

using namespace std::chrono_literals;
using namespace std::string_view_literals;

/**
 * ctx = Graphic::RenderSurface(title, w, h)
 * shader = ctx->createShader(file)
 * vbo = ctx->createBuffer(...)
 * ibo = ctx->createIndexBuffer(...)
 * vao = ctx->createVertex(map, vbo, ibo)
 *
 * ctx->begin()
 * ctx->render(shader, vao)
 * ctx->end()
 */

class Listen
{
public:
    void operator()(Event &e)
    {
        BM_DEBUG("{0}", e.getName());
        EventDispatcher d{ e };
        d.dispatch<WindowCloseEvent>([](WindowCloseEvent &w) { CoreApplication::getApp()->exit(0); });
    }
};
namespace
{
    float ratio(glm::uvec2 value) { return (float)value.x / (float)value.y; }
}// namespace

int main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv)
{
    Log                log;
    GraphicApplication app;
    app.addListener(Listen{});

    auto       module = Graphic::createModule();
    load_guard guard(*module);

    auto window = Graphic::getMainSurface(module);
    window->setWindowName("particle");
    window->make_current();

    Render::ParticleManager manager;
    manager.initialize(100'000);
    glm::mat4 projection = glm::mat4(1.f);

    const auto start = std::chrono::steady_clock::now();

    manager.configure(glm::mat4{}, 0);
    std::mt19937_64                       rand(std::random_device{}());
    std::uniform_real_distribution<float> distribution{ 0, 120 };
    std::uniform_int_distribution<int>    distribution1{ 0, 1 };

    for(unsigned i{}; i < 200; ++i)
        for(unsigned j{}; j < 200; ++j)
            manager.addParticle(
                { (float(i) / 200.f - 0.5f) * 2, (float(j) / 200.f - 0.5f) * 2, 0 },
                { (1.f / 200.f) * 2, (1.f / 200.f) * 2 }, { 0, 0, 0 }, glm::angleAxis(0.f, glm::vec3{ 0, 0, 1 }),
                glm::angleAxis(glm::degrees(5.f * float(distribution1(rand) * 2 - 1)), glm::vec3{ 0, 0, 1 }),
                distribution(rand), glm::vec4{ 1, 0, 0, 1 });


    app.addListener([&](Event &e) {
        EventDispatcher dispatcher{ e };
        dispatcher.dispatch<WindowResizeEvent>([&](WindowResizeEvent &) {
            const auto size  = window->getSize();
            const auto ratio = ::ratio(window->getSize());
            Graphic::RenderCommand::setViewport(0, 0, size.x, size.y);
            projection = glm::ortho(-ratio, ratio, -1.f, 1.f);
        });
    });

    window->show();

    Graphic::RenderCommand::setClearColor(0.15, 0.15, 0.15, 1);


    app.setMainLoop([&] {
        app.processEvent();
        Graphic::RenderCommand::clear();
        manager.configure(
            projection,
            std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::steady_clock::now() - start).count());
        manager.draw();
        window->swap();
    });

    return app.exec();
}