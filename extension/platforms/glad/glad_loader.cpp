#include "glad_loader.hpp"

#include <core/Log.hpp>

#define GLAD_PREFIX(...)    "[GLAD] " __VA_ARGS__
#define GLAD_ERROR(...)     BM_CORE_ERROR(GLAD_PREFIX(__VA_ARGS__))
#define GLAD_CRITICAL(...)  BM_CORE_CRITICAL(GLAD_PREFIX(__VA_ARGS__))
#define GLAD_WARNING(...)   BM_CORE_WARNING(GLAD_PREFIX(__VA_ARGS__))
#define GLAD_TRACE(...)     BM_CORE_WARNING(GLAD_PREFIX(__VA_ARGS__))
#define GLAD_INFO(...)      BM_CORE_INFO(GLAD_PREFIX(__VA_ARGS__))
#define GLAD_ASSERT(X, ...) BM_CUSTOM_ASSERT(GLAD_ERROR, X, __VA_ARGS__)

namespace
{
    void gladCallback(const char *name, void *funcptr [[maybe_unused]], int len_args [[maybe_unused]], ...)
    {
        using namespace std::string_view_literals;
        auto glerrno = glad_glGetError();
        if(glerrno != GL_NO_ERROR)
        {
            std::string_view error;
            switch(glerrno)
            {
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION"sv;
                break;
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM"sv;
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE"sv;
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY"sv;
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION"sv;
                break;
            default:
                error = "undefined error string"sv;
                break;
            }
            GLAD_ERROR("function error call {0} exit with code {1} ({2})", name, glerrno, error);
        }
    }

    void OpenGLMessageCallback(unsigned    source,
                               unsigned    type,
                               unsigned    id,
                               unsigned    severity,
                               int         length,
                               const char *message,
                               const void *userParam)
    {
        switch(severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            GLAD_CRITICAL("{}", message);
            return;
        case GL_DEBUG_SEVERITY_MEDIUM:
            GLAD_ERROR("{}", message);
            return;
        case GL_DEBUG_SEVERITY_LOW:
            GLAD_WARNING("{}", message);
            return;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            GLAD_TRACE("{}", message);
            return;
        }

        GLAD_ERROR("Unknown severity level! {} : {}", id, message);
    }

}// namespace

void GLADLayer::initialize(GLADloadproc loader)
{
    if(!isInitialized)
    {
        isInitialized = gladLoadGLLoader(loader);
        if(!isInitialized)
            throw std::runtime_error("fail to create opengl context");
#ifdef GLAD_DEBUG
        GLAD_INFO("Set glad debug callback");
        glad_set_post_callback(gladCallback);
#endif

#ifndef NDEBUG
        if(glad_glDebugMessageCallback && glad_glDebugMessageControl)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(OpenGLMessageCallback, nullptr);

            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
            GLAD_INFO("Set opengl debug callback");
        }
        else
            GLAD_WARNING("opengl debug not use");
#endif
        GLAD_INFO("Vendor : {0}", glGetString(GL_VENDOR));
        GLAD_INFO("GPU : {0}", glGetString(GL_RENDERER));
        GLAD_INFO("OpenGL version : {0}", glGetString(GL_VERSION));
    }
}
