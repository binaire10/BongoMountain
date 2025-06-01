#include <event/Event.hpp>
#include <event/EventDispatcher.hpp>

#include <core/Layer.hpp>
#include <core/Platform.hpp>
#include <render/RenderDeviceFactory.hpp>
#include <glad_loader.hpp>
#include <GLFW/glfw3.h>
#include <core/Log.hpp>
#include <core/Assert.hpp>

#include <core/config.hpp>

#include <IndexBufferObjectGLPrivate.hpp>
#include <VertexBufferGLPrivate.hpp>
#include <TextureGLPrivate.hpp>
#include <VertexLayoutGLPrivate.hpp>
#include <ShaderGLPrivate.hpp>

#include <event/application/WindowCloseEvent.h>
#include <event/application/WindowResizeEvent.h>

#include <event/keyboard/KeyPressedEvent.h>
#include <event/keyboard/KeyReleasedEvent.h>
#include <event/keyboard/KeyTypedEvent.h>

#include <event/mouse/MouseButtonPressedEvent.h>
#include <event/mouse/MouseButtonReleasedEvent.h>
#include <event/mouse/MouseMovedEvent.h>
#include <event/mouse/MouseScrolledEvent.h>

#define GLFW_PREFIX(...)    "[GLFW] " __VA_ARGS__
#define GLFW_ERROR(...)     BM_CORE_ERROR(GLFW_PREFIX(__VA_ARGS__))
#define GLFW_INFO(...)      BM_CORE_INFO(GLFW_PREFIX(__VA_ARGS__))
#define GLFW_ASSERT(X, ...) BM_CUSTOM_ASSERT(GLFW_ERROR, X, __VA_ARGS__)


namespace
{
    render::RenderDevice *getRenderDevice(GLFWwindow *window)
    {
        return reinterpret_cast<render::RenderDevice *>(glfwGetWindowUserPointer(window));
    }

    void handleResizeEvent(GLFWwindow *window, int width, int height)
    {
        WindowResizeEvent event{ getRenderDevice(window), static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
        core::Platform::getInstance().dispatchEvent(event);
    }

    void handleTypedEvent(GLFWwindow *window, unsigned int keycode)
    {
        KeyTypedEvent event{ getRenderDevice(window), static_cast<KeyCode>(keycode) };
        core::Platform::getInstance().dispatchEvent(event);
    }

    void handleCloseEvent(GLFWwindow *window)
    {
        WindowCloseEvent event{ getRenderDevice(window) };
        core::Platform::getInstance().dispatchEvent(event);
    }

    void handleKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        switch(action)
        {
        case GLFW_PRESS: {
            KeyPressedEvent event(getRenderDevice(window), static_cast<KeyCode>(key), 0u);
            core::Platform::getInstance().dispatchEvent(event);
            break;
        }
        case GLFW_RELEASE: {
            KeyReleasedEvent event(getRenderDevice(window), static_cast<KeyCode>(key));
            core::Platform::getInstance().dispatchEvent(event);
            break;
        }
        case GLFW_REPEAT: {
            KeyPressedEvent event(getRenderDevice(window), static_cast<KeyCode>(key), 1u);
            core::Platform::getInstance().dispatchEvent(event);
            break;
        }
        }
    }

    void handleMouseButtonEvent(GLFWwindow *window, int button, int action, int mods)
    {
        switch(action)
        {
        case GLFW_PRESS: {
            MouseButtonPressedEvent event(getRenderDevice(window), static_cast<MouseCode>(button));
            core::Platform::getInstance().dispatchEvent(event);
            break;
        }
        case GLFW_RELEASE: {
            MouseButtonReleasedEvent event(getRenderDevice(window), static_cast<MouseCode>(button));
            core::Platform::getInstance().dispatchEvent(event);
            break;
        }
        }
    }

    void handleScrollEvent(GLFWwindow *window, double xOffset, double yOffset)
    {
        MouseScrolledEvent event{ getRenderDevice(window), static_cast<float>(xOffset), static_cast<float>(yOffset) };
        core::Platform::getInstance().dispatchEvent(event);
    }

    void handleCursorPosEvent(GLFWwindow *window, double xPos, double yPos)
    {
        MouseMovedEvent event{ getRenderDevice(window), static_cast<float>(xPos), static_cast<float>(yPos) };
        core::Platform::getInstance().dispatchEvent(event);
    }

//    void handleRefreshEvent(GLFWwindow *window)
//    {
//        BM_INFO("refresh");
//        MouseMovedEvent event{ getRenderDevice(window), static_cast<float>(xPos), static_cast<float>(yPos) };
//        core::Platform::getInstance().dispatchEvent(event);
//    }

    class GLFWHandler
    {
    public:
        GLFWHandler(GLFWwindow *window)
            : window(window)
        {}

        ~GLFWHandler() { glfwDestroyWindow(window); }

        void make_current() const { glfwMakeContextCurrent(window); }
        void show() const { glfwShowWindow(window); }
        void flush() const { glfwSwapBuffers(window); }
        void hide() const { glfwHideWindow(window); }

        static void clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); }

        void drawTriangles(const render::VertexBufferObject &object, const render::VertexLayout &layout, unsigned count)
        {
            object.bind();
            layout.bind();
            glDrawArrays(GL_TRIANGLES, 0, count);
        }

        void viewport(glm::vec2 size) { glViewport(0, 0, size.x, size.y); }

        static std::unique_ptr<VertexBufferGLPrivate> createVertexBuffer()
        {
            return std::make_unique<VertexBufferGLPrivate>();
        }

        static std::unique_ptr<VertexBufferGLPrivate> createVertexBuffer(const void *data, std::size_t size)
        {
            auto vb = createVertexBuffer();
            vb->create(data, size);
            return vb;
        }

        static std::unique_ptr<VertexBufferGLPrivate> createVertexBuffer(std::size_t size)
        {
            auto vb = createVertexBuffer();
            vb->create(size);
            return vb;
        }

        static std::unique_ptr<IndexBufferObjectGLPrivate> createIndexBuffer()
        {
            return std::make_unique<IndexBufferObjectGLPrivate>();
        }

        static std::unique_ptr<IndexBufferObjectGLPrivate> createIndexBuffer(const void *data, std::size_t size)
        {
            auto ib = createIndexBuffer();
            ib->create(data, size);
            return ib;
        }

        static std::unique_ptr<IndexBufferObjectGLPrivate> createIndexBuffer(std::size_t size)
        {
            auto ib = createIndexBuffer();
            ib->create(size);
            return ib;
        }

        static std::unique_ptr<TextureGLPrivate> createTexture() { return std::make_unique<TextureGLPrivate>(); }

        static std::unique_ptr<TextureGLPrivate> createTexture(const graphic::ImageView &image)
        {
            auto texture = createTexture();
            texture->create(image);
            return texture;
        }


        static std::unique_ptr<VertexLayoutGLPrivate> createVertexLayout()
        {
            return std::make_unique<VertexLayoutGLPrivate>();
        }
        static std::unique_ptr<VertexLayoutGLPrivate>
            createVertexLayout(const render::AttributeLayout *data, std::size_t count, std::size_t stride)
        {
            auto vertex_layout = createVertexLayout();
            vertex_layout->create(data, count, stride);
            return vertex_layout;
        }

        static std::unique_ptr<ShaderGLPrivate> createShader() { return std::make_unique<ShaderGLPrivate>(); }
        static std::unique_ptr<ShaderGLPrivate> createShader(std::string_view code)
        {
            auto shader = createShader();
            shader->createFromSource(code);
            return shader;
        }

        GLFWwindow *get_native() { return window; }

    private:
        GLFWwindow *window;
    };

    class GLFWRenderDevice final
        : public render::RenderDevice
        , protected GLFWHandler
    {
    public:
        using GLFWHandler::GLFWHandler;
        ~GLFWRenderDevice() override = default;

        void make_current() override { GLFWHandler::make_current(); }
        void flush() override { GLFWHandler::flush(); }
        void clear() override { GLFWHandler::clear(); }
        void drawTriangles(const render::VertexBufferObject &vbo,
                           const render::VertexLayout       &vao,
                           unsigned                          count) override
        {
            GLFWHandler::drawTriangles(vbo, vao, count);
        }
        void                       viewport(glm::vec2 size) override { GLFWHandler::viewport(size); }
        render::VertexBufferObject createVertexBuffer() override { return { this, GLFWHandler::createVertexBuffer() }; }
        render::VertexBufferObject createVertexBuffer(std::size_t size) override
        {
            return { this, GLFWHandler::createVertexBuffer(size) };
        }
        render::VertexBufferObject createVertexBuffer(const void *data, std::size_t size) override
        {
            return { this, GLFWHandler::createVertexBuffer(data, size) };
        }

        render::Texture createTexture() override { return { this, GLFWHandler::createTexture() }; }
        render::Texture createTexture(const graphic::ImageView &image) override
        {
            return { this, GLFWHandler::createTexture(image) };
        }

        render::VertexLayout createVertexLayout() override { return { this, GLFWHandler::createVertexLayout() }; }
        render::VertexLayout
            createVertexLayout(const render::AttributeLayout *data, std::size_t count, std::size_t stride) override
        {
            return { this, GLFWHandler::createVertexLayout(data, count, stride) };
        }

        render::Shader createShader() override { return { this, GLFWHandler::createShader() }; }
        render::Shader createShaderFromCode(std::string_view code) override
        {
            return { this, GLFWHandler::createShader(code) };
        }

        render::IndexBufferObject createIndexBuffer() override { return { this, GLFWHandler::createIndexBuffer() }; }
        render::IndexBufferObject createIndexBuffer(std::size_t size) override
        {
            return { this, GLFWHandler::createIndexBuffer(size) };
        }
        render::IndexBufferObject createIndexBuffer(const void *data, std::size_t size) override
        {
            return { this, GLFWHandler::createIndexBuffer(data, size) };
        }
    };

    class GLFWWindowRenderDevice
        : public render::WindowRenderDevice
        , protected GLFWHandler
    {
    public:
        using GLFWHandler::GLFWHandler;
        ~GLFWWindowRenderDevice() override = default;

        void make_current() override { GLFWHandler::make_current(); }
        void show() override { GLFWHandler::show(); }
        void hide() override { GLFWHandler::hide(); }
        void flush() override { GLFWHandler::flush(); }
        void clear() override { GLFWHandler::clear(); }
        void drawTriangles(const render::VertexBufferObject &vbo,
                           const render::VertexLayout       &vao,
                           unsigned                          count) override
        {
            GLFWHandler::drawTriangles(vbo, vao, count);
        }
        void                       viewport(glm::vec2 size) override { GLFWHandler::viewport(size); }
        render::VertexBufferObject createVertexBuffer() override { return { this, GLFWHandler::createVertexBuffer() }; }
        render::VertexBufferObject createVertexBuffer(std::size_t size) override
        {
            return { this, GLFWHandler::createVertexBuffer(size) };
        }
        render::VertexBufferObject createVertexBuffer(const void *data, std::size_t size) override
        {
            return { this, GLFWHandler::createVertexBuffer(data, size) };
        }


        render::Texture createTexture() override { return { this, GLFWHandler::createTexture() }; }
        render::Texture createTexture(const graphic::ImageView &image) override
        {
            return { this, GLFWHandler::createTexture(image) };
        }

        render::VertexLayout createVertexLayout() override { return { this, GLFWHandler::createVertexLayout() }; }
        render::VertexLayout
            createVertexLayout(const render::AttributeLayout *data, std::size_t count, std::size_t stride) override
        {
            return { this, GLFWHandler::createVertexLayout(data, count, stride) };
        }

        render::Shader createShader() override { return { this, GLFWHandler::createShader() }; }
        render::Shader createShaderFromCode(std::string_view code) override
        {
            return { this, GLFWHandler::createShader(code) };
        }

        render::IndexBufferObject createIndexBuffer() override { return { this, GLFWHandler::createIndexBuffer() }; }
        render::IndexBufferObject createIndexBuffer(std::size_t size) override
        {
            return { this, GLFWHandler::createIndexBuffer(size) };
        }
        render::IndexBufferObject createIndexBuffer(const void *data, std::size_t size) override
        {
            return { this, GLFWHandler::createIndexBuffer(data, size) };
        }
    };

    void handleError(int errorCode, const char *errorMsg) noexcept
    {
        GLFW_ERROR("GLFW error [{0}] : {1}", errorCode, errorMsg);
    }

    class GLFW_LAYER final : public core::Layer
    {
    public:
        constexpr GLFW_LAYER()
            : Layer{ -100 }
        {}

        void onAttach() override
        {
            GLFW_INFO("Attach");
            GLFW_INFO("compile version {}.{}.{}", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);
            int major, minor, revision;
            glfwGetVersion(&major, &minor, &revision);
            GLFW_INFO("runtime version {}.{}.{}", major, minor, revision);
            glfwSetErrorCallback(handleError);

            GLFW_ASSERT(GLFW_VERSION_MAJOR == major and GLFW_VERSION_MINOR <= minor, "Binary file incompatible!")
            auto status = glfwInit();

            GLFW_ASSERT(status, "Failed to initialize GLFW!");
        }
        void onDetach() override
        {
            GLFW_INFO("Detach");
            glfwTerminate();
        }
        void onBegin() override { glfwPollEvents(); }
        void onEnd() override {}
        void onUpdate() override {}
        void onEvent(Event &event) override {}
    };

    template<typename LoaderGL>
    class GLFWFactory
        : public render::RenderDeviceFactory
        , private LoaderGL
    {
    public:
        std::unique_ptr<render::RenderDevice> createOpenGLRenderDevice(const render::RenderDeviceInfo &query,
                                                                       const render::opengl_t         *render)
        {
            glfwDefaultWindowHints();
            glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
            if(render->version)
            {
                auto &version = *render->version;
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, int(version[0]));
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, int(version[1]));
                GLFW_INFO("Create OpenGL {}.{} in {}",
                          int(version[0]),
                          int(version[1]),
                          render->isCoreProfile ? "Core Profile" : "Compatible Profile");
                if(render->isCoreProfile)
                    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_CORE_PROFILE);
                //                else
                //                    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_COMPAT_PROFILE);
            }
            glfwWindowHint(GLFW_VISIBLE, query.window && query.window->isVisible ? 1 : 0);

            std::string_view title = query.window ? query.window->name : "";
            GLFWwindow      *win   = fixe_string(
                [&query](auto str) {
                    return glfwCreateWindow(query.width, query.height, str.data(), nullptr, nullptr);
                },
                title);
            if(!win)
                return nullptr;

            glfwMakeContextCurrent(win);

            LoaderGL::initialize(glfwGetProcAddress);

            std::unique_ptr<render::RenderDevice> device;
            if(query.window)
                device = std::make_unique<GLFWWindowRenderDevice>(win);
            else
                device = std::make_unique<GLFWRenderDevice>(win);

            glfwSetWindowUserPointer(win, device.get());

            glfwSetWindowSizeCallback(win, handleResizeEvent);
            glfwSetWindowCloseCallback(win, handleCloseEvent);
            glfwSetKeyCallback(win, handleKeyEvent);
            glfwSetCharCallback(win, handleTypedEvent);
            glfwSetMouseButtonCallback(win, handleMouseButtonEvent);
            glfwSetScrollCallback(win, handleScrollEvent);
            glfwSetCursorPosCallback(win, handleCursorPosEvent);
//            glfwSetWindowRefreshCallback(win, handleRefreshEvent);

            return device;
        }

        std::unique_ptr<render::RenderDevice> createRenderDevice(const render::RenderDeviceInfo &info) override
        {
            const render::opengl_t *render = std::get_if<render::opengl_t>(&info.render);
            if(info.render.valueless_by_exception())
                return nullptr;
            return createOpenGLRenderDevice(info, render);
        }
    };
}// namespace

extern "C"
{
[[maybe_unused]] BM_EXPORT_DCL void initialize(core::RepositoryBindings &e, const nlohmann::json &configs)
{
    using namespace core;
    using module_binding = factory::define_type<GLFW_LAYER()>::bind<core::Layer>::make;
    using render_binding = factory::define_type<GLFWFactory<GLADLayer>()>::bind<render::RenderDeviceFactory>::make;
    e.register_factory<make_factory::add<module_binding, render_binding>::build>();
    //    e.addLayer(std::make_unique<Module<GLADLayer>>());
}
}
