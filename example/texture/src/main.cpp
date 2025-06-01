// #include <Core/Log.hpp>
// #include <Core/load_guard.hpp>
// #include <Event/Application/WindowCloseEvent.h>
// #include <Event/Application/WindowResizeEvent.h>
// #include <Event/EventDispatcher.h>
// #include <Graphic/BufferLayout.h>
// #include <Graphic/GraphicApplication.hpp>
// #include <Graphic/Module.hpp>
// #include <thread>
//
// using namespace std::chrono_literals;
// using namespace std::string_view_literals;
//
///**
// * ctx = Graphic::RenderSurface(title, w, h)
// * shader = ctx->createShader(file)
// * vbo = ctx->createBuffer(...)
// * ibo = ctx->createIndexBuffer(...)
// * vao = ctx->createVertex(map, vbo, ibo)
// *
// * ctx->begin()
// * ctx->render(shader, vao)
// * ctx->end()
// */
//
// class Listen
//{
// public:
//    void operator()(Event &e)
//    {
//        BM_DEBUG("{0}", e.getName());
//        EventDispatcher d{ e };
//        d.dispatch<WindowCloseEvent>([](WindowCloseEvent &w) { CoreApplication::getApp()->exit(0); });
//    }
//};
// namespace
//{
//    float ratio(glm::uvec2 value) { return (float)value.x / (float)value.y; }
//}// namespace
//
// int main(int argc, const char **argv)
//{
//    static constexpr std::string_view code =
//
//        static constexpr BufferLayout layout = make_layout(BufferElement{ ShaderDataType::Float3, "a_Position" },
//                                                           BufferElement{ ShaderDataType::Float2, "a_TexCoord" });
//    Log                               log;
//    GraphicApplication                app;
//    app.addListener(Listen{});
//
//    auto       module = Graphic::createModule();
//    load_guard guard(*module);
//
//    auto window = Graphic::getMainSurface(module);
//    window->setWindowName("texture");
//    window->show();
//    window->make_current();
//
//    static constexpr float squareVertices[5 * 4] = { -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
//                                                     0.5f,  0.5f,  0.0f, 1.0f, 1.0f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f };
//
//    static constexpr unsigned indices[]{ 0, 1, 2, 2, 3, 0 };
//
//    auto texture = Graphic::Texture::load_texture("assets/Checkerboard.png");
//    auto shader  = Graphic::Shader::createFromCode(code);
//
//    auto vao = Graphic::VertexArray<unsigned>::create(layout,
//                                                      Graphic::SharedIndexBufferArray<unsigned>::create(indices),
//                                                      Graphic::SharedVertexBuffer ::create(squareVertices));
//
//    auto view      = shader.getUniformLocation("u_ViewProjection");
//    auto transform = shader.getUniformLocation("u_Transform");
//    // auto textUnit = shader.getUniformLocation("u_Texture");
//
//    shader.bind();
//    {
//        const auto ratio = ::ratio(window->getSize());
//
//        shader.setUniformValue(view, glm::ortho(-ratio, ratio, -1.f, 1.f));
//        shader.setUniformValue(transform, glm::mat4{ 1.f });
//        // shader.setUniformValue(textUnit, 0);
//    }
//    app.addListener([&](Event &e) {
//        EventDispatcher dispatcher{ e };
//        dispatcher.dispatch<WindowResizeEvent>([&](WindowResizeEvent &) {
//            const auto size  = window->getSize();
//            const auto ratio = ::ratio(window->getSize());
//            Graphic::RenderCommand::setViewport(0, 0, size.x, size.y);
//            shader.setUniformValue(view, glm::ortho(-ratio, ratio, -1.f, 1.f));
//        });
//    });
//
//    Graphic::RenderCommand::setClearColor(0.15, 0.15, 0.15, 1);
//
//    app.setMainLoop([&] {
//        app.processEvent();
//        Graphic::RenderCommand::clear();
//
//        texture.bind(/*0*/);
//        Graphic::RenderCommand::drawIndexed(shader, vao);
//
//        window->swap();
//    });
//
//    return app.exec();
//}

#include <glm/gtc/matrix_transform.hpp>
#include <render/VertexLayoutDescriptor.h>
#include <fstream>
#include <core/Platform.hpp>
#include <core/Log.hpp>
#include <event/Event.hpp>
#include <event/EventDispatcher.hpp>
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
    static constexpr float squareVertices[5 * 4] = { -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
                                                     0.5f,  0.5f,  0.0f, 1.0f, 1.0f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f };

    static constexpr uint16_t indices[]{ 0, 1, 2, 2, 3, 0 };

    static constexpr render::RenderDeviceInfo deviceInfo{ 1600, 1200, render::opengl_t{},
                                                          render::window_t{ .name = "texture", .isVisible = true } };
    static constexpr auto                     layout =
        render::make_layout(render::AttributeLayout{ "a_Position", render::ShaderDataType::Float3 },
                            render::AttributeLayout{ "a_TexCoord", render::ShaderDataType::Float2 });

    static constexpr std::string_view shaderSource =
        "#type vertex\n"
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec3 a_Position;\n"
        "layout(location = 1) in vec2 a_TexCoord;\n"
        "\n"
        "uniform mat4 u_ViewProjection;\n"
        "uniform mat4 u_Transform;\n"
        "\n"
        "out vec2 v_TexCoord;\n"
        "\n"
        "void main()\n"
        "{\n"
        "\tv_TexCoord = a_TexCoord;\n"
        "\tgl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);\n"
        "}\n"
        "\n"
        "#type fragment\n"
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "in vec2 v_TexCoord;\n"
        "\n"
        "uniform sampler2D u_Texture;\n"
        "\n"
        "void main()\n"
        "{\n"
        "\tcolor = texture(u_Texture, v_TexCoord);\n"
        "}\n";

    void onAttach() override
    {
        device = graphic::createRenderDevice(core::Platform::getInstance(), deviceInfo);
        if(!device)
            return;

        vbo    = device->createVertexBuffer(squareVertices);
        vao    = device->createVertexLayout(layout);
        ibo    = device->createIndexBuffer(indices);
        shader = device->createShaderFromCode(shaderSource);
        std::filebuf filebuf;
        filebuf.open("assets/Checkerboard.png", std::ios::binary | std::ios::in);
        if(auto checkboard = graphic::ImageLoader{}.load("assets/Checkerboard.png", &filebuf))
            texture = device->createTexture(*checkboard);
        start = std::chrono::high_resolution_clock::now();

        float ratio = (float)deviceInfo.width / deviceInfo.height;

        shader.bind();
        view      = shader.getUniformLocation("u_ViewProjection");
        transform = shader.getUniformLocation("u_Transform");

        shader.setUniformValue(view, glm::ortho(-ratio, ratio, -1.f, 1.f));
        shader.setUniformValue(transform, glm::mat4{ 1.f });

        device->setClearColor(0.15, 0.15, 0.15, 1);
    }

    void onDetach() override
    {
        texture.destroy();
        vao.destroy();
        vbo.destroy();
        ibo.destroy();
        shader.destroy();
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
        texture.bind();
        device->drawTrianglesWithIndex(vbo, vao, ibo, 6);
    }

    void onEvent(Event &event) override
    {
        EventDispatcher dispatcher{ event };
        dispatcher.dispatch<WindowResizeEvent>([this](const WindowResizeEvent &event) {
            device->viewport(glm::vec2(event.getWidth(), event.getHeight()));
            float ratio = (float)event.getWidth() / event.getHeight();
            shader.bind();
            shader.setUniformValue(view, glm::ortho(-ratio, ratio, -1.f, 1.f));
            core::Platform::getInstance().compute();
        });
        dispatcher.dispatch<WindowCloseEvent>([](auto &e) { core::Platform::getInstance().exit(); });
    }

private:
    std::unique_ptr<render::RenderDevice>          device;
    render::VertexBufferObject                     vbo;
    render::IndexBufferObjectTyped<uint16_t>       ibo;
    render::VertexLayout                           vao;
    render::Shader                                 shader;
    render::Texture                                texture;
    std::chrono::high_resolution_clock::time_point start;

    int view      = -1;
    int transform = -1;
};

int main(int argc, const char **argv)
{
    core::Log      log;
    core::Platform platform{ argc, argv };
    platform.load(platform.getApplicationPath().parent_path().append("extension"));

    platform.addLayer(std::make_shared<CustomLayer>());

    platform.exec();
}