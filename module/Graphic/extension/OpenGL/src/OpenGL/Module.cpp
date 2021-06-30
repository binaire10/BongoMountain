#include "Module.hpp"

#include <Core/Log.hpp>
#include "config.hpp"
#if defined(BM_PLATFORM_GLFW)
#    include <GLFW/glfw3.h>
#    include <Core/Assert.hpp>
#endif

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
            BM_CORE_ERROR("[GLAD] function error call {0} exit with code {1} ({2})", name, glerrno, error);
        }
    }
}// namespace

void OpenGL::Module::load(Graphic::GlobalContext &)
{
#ifdef BM_PLATFORM_GLAD
    BM_CORE_INFO("Initialize GLAD");
#    ifndef NDEBUG
    BM_CORE_INFO("Set debug callback");
    glad_set_post_callback(gladCallback);
#    endif
#    if defined(BM_PLATFORM_GLFW)
    int status [[maybe_unused]] = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    BM_CORE_ASSERT(status, "Failed to initialize Glad with glfw")
#    endif
#endif
    BM_CORE_INFO("Vendor : {0}", glGetString(GL_VENDOR));
    BM_CORE_INFO("GPU : {0}", glGetString(GL_RENDERER));
    BM_CORE_INFO("OpenGL version : {0}", glGetString(GL_VERSION));
}
