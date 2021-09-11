#include "Module.hpp"

#include <Core/Log.hpp>
#include "config.hpp"
#if defined(BM_PLATFORM_GLFW)
#    include <GLFW/glfw3.h>
#    include <Core/Assert.hpp>
#endif

namespace
{
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
            BM_CORE_CRITICAL(message);
            return;
        case GL_DEBUG_SEVERITY_MEDIUM:
            BM_CORE_ERROR(message);
            return;
        case GL_DEBUG_SEVERITY_LOW:
            BM_CORE_WARNING(message);
            return;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            BM_CORE_TRACE(message);
            return;
        }

        BM_CORE_ASSERT(false, "Unknown severity level!");
    }

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
            BM_CORE_ERROR("[GLAD] function error call {0} exit with code {1} ({2})", name, glerrno, error);
        }
    }
}// namespace

void OpenGL::Module::load(Graphic::GlobalContext &)
{
#ifdef BM_PLATFORM_GLAD
    BM_CORE_INFO("Initialize GLAD");
#    ifdef GLAD_DEBUG
    BM_CORE_INFO("Set glad debug callback");
    glad_set_post_callback(gladCallback);
#    endif
#    if defined(BM_PLATFORM_GLFW)
    int status [[maybe_unused]] = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    BM_CORE_ASSERT(status, "Failed to initialize Glad with glfw")
#    endif
#endif
#ifndef NDEBUG
#    ifdef GLAD_DEBUG
    if(glad_glDebugMessageCallback && glad_glDebugMessageControl)
#    else
    if(glDebugMessageCallback && glDebugMessageControl)
#    endif
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OpenGLMessageCallback, nullptr);

        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
        BM_CORE_INFO("Set opengl debug callback");
    }
    else
        BM_CORE_WARNING("opengl debug not use");
#endif
    BM_CORE_INFO("Vendor : {0}", glGetString(GL_VENDOR));
    BM_CORE_INFO("GPU : {0}", glGetString(GL_RENDERER));
    BM_CORE_INFO("OpenGL version : {0}", glGetString(GL_VERSION));
}
