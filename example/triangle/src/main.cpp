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
    static constexpr ColoredVertex array_data[]{ ColoredVertex{ { -0.5f, -0.5f, 0.0f }, { 0.8f, 0.2f, 0.8f, 1.0f } },
                                                 ColoredVertex{ { 0.5f, -0.5f, 0.0f }, { 0.2f, 0.3f, 0.8f, 1.0f } },
                                                 ColoredVertex{ { 0.0f, 0.5f, 0.0f }, { 0.8f, 0.8f, 0.2f, 1.0f } } };
    // static constexpr float vertices[3 * 7]{ -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f, 0.5f, -0.5f, 0.0f, 0.2f,
    //                                          0.3f,  0.8f, 1.0f, 0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f,  1.0f };

    static constexpr uint8_t indices[]{ 0, 1, 2 };

    static constexpr render::RenderDeviceInfo deviceInfo{ 1600, 1200, render::opengl_t{},
                                                          render::window_t{ .name = "triangle", .isVisible = true } };
    static constexpr auto                     layout =
        render::make_layout(render::AttributeLayout{ "a_Position", render::ShaderDataType::Float3 },
                            render::AttributeLayout{ "a_Color", render::ShaderDataType::Float4 });

    static constexpr std::string_view shaderSource =
        "#type vertex\n"
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec3 a_Position;\n"
        "layout(location = 1) in vec4 a_Color;\n"
        "uniform mat4 u_ViewProjection;\n"
        "uniform mat4 u_Transform;\n"
        "out vec3 v_Position;\n"
        "out vec4 v_Color;\n"
        "void main()\n"
        "{\n"
        "\tv_Position = a_Position;\n"
        "\tv_Color = a_Color;\n"
        "\tgl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);\t\n"
        "}\n"
        "#type fragment\n"
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "in vec3 v_Position;\n"
        "in vec4 v_Color;\n"
        "void main()\n"
        "{\n"
        "\tcolor = vec4(v_Position * 0.5 + 0.5, 1.0);\n"
        "\tcolor = v_Color;\n"
        "}";

    void onAttach() override
    {
        device = graphic::createRenderDevice(core::Platform::getInstance(), deviceInfo);
        if(!device)
            return;

        vbo = device->createVertexBuffer(6 * sizeof(ColoredVertex));
        vbo.insert(0, array_data);
        ibo    = device->createIndexBuffer(indices);
        vao    = device->createVertexLayout(layout);
        shader = device->createShaderFromCode(shaderSource);
        start  = std::chrono::high_resolution_clock::now();

        float ratio = (float)deviceInfo.width / deviceInfo.height;

        constexpr glm::vec3 change{ 0.0f, 0.9f, 0.0f };
        vbo.overwrite(2 * sizeof(ColoredVertex), &change, 1);

        shader.bind();
        view      = shader.getUniformLocation("u_ViewProjection");
        transform = shader.getUniformLocation("u_Transform");

        shader.setUniformValue(view, glm::ortho(-ratio, ratio, -1.f, 1.f));
        shader.setUniformValue(transform, glm::mat4{ 1.f });

        device->setClearColor(0.15, 0.15, 0.15, 1);
    }

    void onDetach() override
    {
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
        ibo.bind();
        device->drawTrianglesWithIndex(vbo, vao, ibo, vbo.size() / sizeof(ColoredVertex));
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
    render::IndexBufferObjectTyped<uint8_t>        ibo;
    render::VertexLayout                           vao;
    render::Shader                                 shader;
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