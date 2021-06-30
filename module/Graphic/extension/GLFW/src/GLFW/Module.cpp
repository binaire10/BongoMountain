#include "Module.hpp"
#include <Core/Assert.hpp>
#include <GLFW/glfw3.h>

namespace
{
    void handleError(int errorCode, const char *errorMsg) noexcept
    {
        BM_CORE_ERROR("GLFW error [{0}] : {1}", errorCode, errorMsg);
    }
}// namespace

void glfw::Module::load(Context &ctx)
{
    BM_CORE_INFO("GLFW compile version {}.{}.{}", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);
    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    BM_CORE_INFO("GLFW runtime version {}.{}.{}", major, minor, revision);
    BM_CORE_INFO("Initialize GLFW");
    glfwSetErrorCallback(handleError);

    BM_CORE_ASSERT(GLFW_VERSION_MAJOR == major and GLFW_VERSION_MINOR <= minor, "Binary file incompatible!")
    auto status = glfwInit();
    BM_CORE_ASSERT(status, "Failed to initialize GLFW!")

    //        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    ctx.mainSurface.emplace("Bongo Mountain Engine", 1280, 720);
    ctx.mainSurface->make_current();
}

void glfw::Module::unload(glfw::Context &ctx) noexcept
{
    ctx.mainSurface.reset();
    glfwTerminate();
}
