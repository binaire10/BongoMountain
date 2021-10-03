#include <Core/Log.hpp>
#include <Core/load_guard.hpp>
#include <Event/Application/WindowCloseEvent.h>
#include <Event/EventDispatcher.h>
#include <Graphic/BufferLayout.h>
#include <Graphic/GraphicApplication.hpp>
#include <Graphic/Module.hpp>
#include <Render/Render2D.hpp>
#include <thread>

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

int main(int argc, const char **argv)
{
    static constexpr std::string_view code =
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

    static constexpr BufferLayout layout = make_layout(BufferElement{ ShaderDataType::Float3, "a_Position" },
                                                       BufferElement{ ShaderDataType::Float4, "a_Color" });
    Log                           log;
    GraphicApplication            app;
    app.addListener(Listen{});

    auto       module = Graphic::createModule();
    load_guard guard(*module);

    auto window = Graphic::getMainSurface(module);
    window->setWindowName("triangle");
    window->show();
    window->make_current();

    static constexpr float vertices[3 * 7]{ -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f, 0.5f, -0.5f, 0.0f, 0.2f,
                                            0.3f,  0.8f,  1.0f, 0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f,  1.0f };

    static constexpr unsigned indices[]{ 0, 1, 2 };

    auto renderCommand = window->getRenderCommand();
    auto shader        = renderCommand->createShader(code);

    auto vao = renderCommand->createVertexArray(
        layout, renderCommand->createBufferIndexStorage(indices), renderCommand->createBuffer(vertices));

    auto view      = shader.getUniformLocation("u_ViewProjection");
    auto transform = shader.getUniformLocation("u_Transform");

    shader.bind();
    shader.setUniformValue(view, glm::ortho(-1.f, 1.f, -1.f, 1.f));
    shader.setUniformValue(transform, glm::mat4{ 1.f });


    renderCommand->setClearColor(0.15, 0.15, 0.15, 1);
    Render2D render{renderCommand, 20000*4, 20000};

    app.setMainLoop([&] {
        app.processEvent();
        renderCommand->clear();

        renderCommand->renderTriangle(shader, vao);

        window->swap();
    });

    return app.exec();
}