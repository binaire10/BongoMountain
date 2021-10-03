#include <Core/Log.hpp>
#include <Core/load_guard.hpp>
#include <Event/Application/WindowCloseEvent.h>
#include <Event/Application/WindowResizeEvent.h>
#include <Event/EventDispatcher.h>
#include <Graphic/BufferLayout.h>
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

int main(int argc, const char **argv)
{
    static constexpr std::string_view code =
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
        "color = vec4(color.r, 1.0, 1.0, 1.0);"
        "}\n";

    static constexpr BufferLayout layout = make_layout(BufferElement{ ShaderDataType::Float3, "a_Position" },
                                                       BufferElement{ ShaderDataType::Float2, "a_TexCoord" });
    Log                           log;
    GraphicApplication            app;
    app.addListener(Listen{});

    auto       module = Graphic::createModule();
    load_guard guard(*module);

    auto window = Graphic::getMainSurface(module);
    window->setWindowName("texture");
    window->show();
    window->make_current();

    static constexpr float squareVertices[5 * 4] = { -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
                                                     0.5f,  0.5f,  0.0f, 1.0f, 1.0f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f };

    static constexpr unsigned indices[]{ 0, 1, 2, 2, 3, 0 };

    auto texture = Graphic::Texture::load_texture("assets/Checkerboard.png");
    auto shader  = Graphic::Shader::createFromCode(code);

    auto vao = Graphic::VertexArray<unsigned>::create(layout,
                                                      Graphic::SharedIndexBufferArray<unsigned>::create(indices),
                                                      Graphic::SharedVertexBuffer ::create(squareVertices));

    auto view      = shader.getUniformLocation("u_ViewProjection");
    auto transform = shader.getUniformLocation("u_Transform");
//    auto textUnit = shader.getUniformLocation("u_Texture");

    shader.bind();
    {
        const auto ratio = ::ratio(window->getSize());

        shader.setUniformValue(view, glm::ortho(-ratio, ratio, -1.f, 1.f));
        shader.setUniformValue(transform, glm::mat4{ 1.f });
//        shader.setUniformValue(textUnit, 0);
    }
    app.addListener([&](Event &e) {
        EventDispatcher dispatcher{ e };
        dispatcher.dispatch<WindowResizeEvent>([&](WindowResizeEvent &) {
            const auto size  = window->getSize();
            const auto ratio = ::ratio(window->getSize());
            Graphic::RenderCommand::setViewport(0, 0, size.x, size.y);
            shader.setUniformValue(view, glm::ortho(-ratio, ratio, -1.f, 1.f));
        });
    });

    Graphic::RenderCommand::setClearColor(0.15, 0.15, 0.15, 1);

    app.setMainLoop([&] {
        app.processEvent();
        Graphic::RenderCommand::clear();

        texture.bind(/*0*/);
        Graphic::RenderCommand::drawIndexed(shader, vao);

        window->swap();
    });

    return app.exec();
}