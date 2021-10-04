#include <Core/Log.hpp>
#include <Core/load_guard.hpp>
#include <Event/Application/WindowCloseEvent.h>
#include <Event/Application/WindowResizeEvent.h>
#include <Event/EventDispatcher.h>
#include <Graphic/BufferLayout.h>
#include <Graphic/BufferVector.hpp>
#include <Graphic/GraphicApplication.hpp>
#include <Graphic/Module.hpp>
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

struct ColoredVertex
{
    glm::vec3 pos;
    glm::vec4 color;
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

    auto       module = Graphic::createModule();
    load_guard guard(*module);

    auto window = Graphic::getMainSurface(module);
    window->setWindowName("triangle");
    window->make_current();

    static constexpr ColoredVertex vertices[]{ ColoredVertex{ { -0.5f, -0.5f, 0.0f }, { 0.8f, 0.2f, 0.8f, 1.0f } },
                                               ColoredVertex{ { 0.5f, -0.5f, 0.0f }, { 0.2f, 0.3f, 0.8f, 1.0f } },
                                               ColoredVertex{ { 0.0f, 0.5f, 0.0f }, { 0.8f, 0.8f, 0.2f, 1.0f } } };
    // static constexpr float vertices[3 * 7]{ -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f, 0.5f, -0.5f, 0.0f, 0.2f,
    //                                          0.3f,  0.8f, 1.0f, 0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f,  1.0f };

    static constexpr uint8_t indices[]{ 0, 1, 2 };

    auto shader = Graphic::Shader::createFromCode(code);

    auto test = Graphic::SharedVertexBufferVector<ColoredVertex>::create(6);
    test.insert_back(vertices, 3);
    {
        const auto &container = test.getContainer();
        auto        array     = container.getData<const ColoredVertex>();
        for(unsigned i{}; i < test.getCount(); ++i)
        {
            const auto &pos = array[i].pos;
            BM_TRACE("Point({}, {}, {})", pos.x, pos.y, pos.z);
        }
    }

    auto vao = Graphic::VertexArray<std::decay_t<decltype(*indices)>>::create(
        layout, Graphic::SharedIndexBufferArray<uint8_t>::create(indices), test.getContainer());
    constexpr ColoredVertex change{ { 0.0f, 0.9f, 0.0f }, { 0.8f, 0.8f, 0.2f, 1.0f } };
    test.set(2, change);

    auto view      = shader.getUniformLocation("u_ViewProjection");
    auto transform = shader.getUniformLocation("u_Transform");

    shader.bind();
    shader.setUniformValue(view, glm::ortho(-1.f, 1.f, -1.f, 1.f));
    shader.setUniformValue(transform, glm::mat4{ 1.f });

    BM_TRACE("mesh(count={}, capacity={})", test.getCount(), test.getCapacity());

    app.addListener([view, &shader](Event &e) {
        BM_DEBUG("{0}", e.getName());
        EventDispatcher d{ e };
        d.dispatch<WindowResizeEvent>([&](WindowResizeEvent &w) {
            Graphic::RenderCommand::setViewport(0, 0, w.getWidth(), w.getHeight());
            float ratio = (float)w.getWidth() / w.getHeight();
            shader.bind();
            shader.setUniformValue(view, glm::ortho(-ratio, ratio, -1.f, 1.f));
        });
        d.dispatch<WindowCloseEvent>([](WindowCloseEvent &w) { CoreApplication::getApp()->exit(0); });
    });

    Graphic::RenderCommand::setClearColor(0.15, 0.15, 0.15, 1);

    app.setMainLoop([&] {
        app.processEvent();
        Graphic::RenderCommand::clear();

        Graphic::RenderCommand::drawIndexed(shader, vao);

        window->swap();
    });

    window->show();
    return app.exec();
}