#include <event/Event.hpp>
#include <event/EventDispatcher.hpp>

#include <core/Layer.hpp>
#include <core/Platform.hpp>
#include <render/event/QueryRenderDevice.hpp>
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

#define GLFW_PREFIX(...)    "[GLFW] " __VA_ARGS__
#define GLFW_ERROR(...)     BM_CORE_ERROR(GLFW_PREFIX(__VA_ARGS__))
#define GLFW_INFO(...)      BM_CORE_INFO(GLFW_PREFIX(__VA_ARGS__))
#define GLFW_ASSERT(X, ...) BM_CUSTOM_ASSERT(GLFW_ERROR, X, __VA_ARGS__)

namespace
{
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

        void draw(const render::VertexBufferObject &object, const render::VertexLayout &layout, unsigned count)
        {
            object.bind();
            layout.bind();
            glDrawArrays(GL_TRIANGLES, 0, count);
        }

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

    class GLFWRenderDevice
        : public render::RenderDevice
        , protected GLFWHandler
    {
    public:
        using GLFWHandler::GLFWHandler;
        ~GLFWRenderDevice() override = default;

        void make_current() override { GLFWHandler::make_current(); }
        void flush() override { GLFWHandler::flush(); }
        void clear() override { GLFWHandler::clear(); }
        void draw(const render::VertexBufferObject &vbo, const render::VertexLayout &vao, unsigned count) override
        {
            GLFWHandler::draw(vbo, vao, count);
        }

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
        void draw(const render::VertexBufferObject &vbo, const render::VertexLayout &vao, unsigned count) override
        {
            GLFWHandler::draw(vbo, vao, count);
        }

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

    template<typename LoaderGL>
    class Module
        : public core::Layer
        , private LoaderGL
    {
    public:
        constexpr Module()
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
        void onEvent(Event &event) override
        {
            if(event.IsInCategory(EventCategory::Layer))
            {
                EventDispatcher dispatcher{ event };
                dispatcher.dispatch<render::event::QueryRenderDevice>([this](render::event::QueryRenderDevice &query) {
                    auto *info = std::get_if<render::RenderDeviceInfo>(&query.getInfo());
                    if(info)
                    {
                        query.setRenderDevice(createRenderDevice(*info));
                        query.setHandle(bool(query.getRenderDevice()));
                    }
                });
            }
        }

    protected:
        std::unique_ptr<render::RenderDevice> createRenderDevice(const render::RenderDeviceInfo &query)
        {
            const render::opengl_t *render = std::get_if<render::opengl_t>(&query.render);
            if(!render)
                return nullptr;
            return createOpenGLRenderDevice(query, render);
        }

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
                else
                    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_COMPAT_PROFILE);
            }
            glfwWindowHint(GLFW_VISIBLE, query.window && query.window->isVisible ? 1 : 0);

            std::string_view title = query.window ? query.window->name : "";
            GLFWwindow      *win   = fixe_string(title, [&query](auto str) {
                return glfwCreateWindow(query.width, query.height, str.data(), nullptr, nullptr);
                   });
            if(!win)
                return nullptr;

            glfwMakeContextCurrent(win);

            LoaderGL::initialize(glfwGetProcAddress);

            if(query.window)
                return std::make_unique<GLFWWindowRenderDevice>(win);
            else
                return std::make_unique<GLFWRenderDevice>(win);
        }
    };
}// namespace

extern "C" {
BM_EXPORT_DCL void initialize(core::ExtensionManager &e, const nlohmann::json &configs)
{
    e.addLayer(std::make_unique<Module<GLADLayer>>());
}
}
