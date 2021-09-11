#include "RenderSurface.hpp"
#include <Core/memory.hpp>
#include <GLFW/glfw3.h>

#include <GLFW/glfw3.h>

#include <Core/Log.hpp>
#include <Core/Assert.hpp>
#include <Core/CoreApplication.hpp>
#include <Graphic/EventWrapper.hpp>
#include <Event/Application/WindowCloseEvent.h>
#include <Event/Application/WindowResizeEvent.h>
#include <Event/Keyboard/KeyPressedEvent.h>
#include <Event/Keyboard/KeyReleasedEvent.h>
#include <Event/Keyboard/KeyTypedEvent.h>
#include <Event/Mouse/MouseButtonReleasedEvent.h>
#include <Event/Mouse/MouseButtonPressedEvent.h>
#include <Event/Mouse/MouseMovedEvent.h>
#include <Event/Mouse/MouseScrolledEvent.h>

namespace
{
    Graphic::RenderSurface *getSurface(GLFWwindow *window)
    {
        return reinterpret_cast<Graphic::RenderSurface *>(glfwGetWindowUserPointer(window));
    }

    void handleResizeEvent(GLFWwindow *window, int width, int height)
    {
        WindowResizeEvent event{ getSurface(window), static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
        CoreApplication::dispatchEvent(event);
    }

    void handleTypedEvent(GLFWwindow *window, unsigned int keycode)
    {
        KeyTypedEvent event{ getSurface(window), static_cast<KeyCode>(keycode) };
        CoreApplication::dispatchEvent(event);
    }

    void handleCloseEvent(GLFWwindow *window)
    {
        WindowCloseEvent event{ getSurface(window) };
        CoreApplication::dispatchEvent(event);
    }

    void handleKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        switch(action)
        {
        case GLFW_PRESS: {
            KeyPressedEvent event(getSurface(window), static_cast<KeyCode>(key), 0u);
            CoreApplication::dispatchEvent(event);
            break;
        }
        case GLFW_RELEASE: {
            KeyReleasedEvent event(getSurface(window), static_cast<KeyCode>(key));
            CoreApplication::dispatchEvent(event);
            break;
        }
        case GLFW_REPEAT: {
            KeyPressedEvent event(getSurface(window), static_cast<KeyCode>(key), 1u);
            CoreApplication::dispatchEvent(event);
            break;
        }
        }
    }

    void handleMouseButtonEvent(GLFWwindow *window, int button, int action, int mods)
    {
        switch(action)
        {
        case GLFW_PRESS: {
            MouseButtonPressedEvent event(getSurface(window), static_cast<MouseCode>(button));
            CoreApplication::dispatchEvent(event);
            break;
        }
        case GLFW_RELEASE: {
            MouseButtonReleasedEvent event(getSurface(window), static_cast<MouseCode>(button));
            CoreApplication::dispatchEvent(event);
            break;
        }
        }
    }

    void handleScrollEvent(GLFWwindow *window, double xOffset, double yOffset)
    {
        MouseScrolledEvent event{ getSurface(window), static_cast<float>(xOffset), static_cast<float>(yOffset) };
        CoreApplication::dispatchEvent(event);
    }

    void handleCursorPosEvent(GLFWwindow *window, double xPos, double yPos)
    {
        MouseMovedEvent event{ getSurface(window), static_cast<float>(xPos), static_cast<float>(yPos) };
        CoreApplication::dispatchEvent(event);
    }
}// namespace

namespace Graphic
{
    struct RenderSurfacePrivate
    {
        constexpr explicit RenderSurfacePrivate(GLFWwindow *win) noexcept
            : win(win)
        {}

        ~RenderSurfacePrivate() noexcept { glfwDestroyWindow(win); }
        GLFWwindow *win;
    };
}// namespace Graphic

Graphic::RenderSurface::RenderSurface(std::string_view title, unsigned int width, unsigned int height)
{
#if !defined(NDEBUG) && defined(BM_PLATFORM_OPENGL)
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

    data = fixe_string(title, [=](std::string_view str) { return create(str, width, height); });

    glfwDefaultWindowHints();

    BM_CORE_ASSERT(data->win, "Failed to create window!")

    glfwSetWindowSizeCallback(data->win, handleResizeEvent);
    glfwSetWindowCloseCallback(data->win, handleCloseEvent);
    glfwSetKeyCallback(data->win, handleKeyEvent);
    glfwSetCharCallback(data->win, handleTypedEvent);
    glfwSetMouseButtonCallback(data->win, handleMouseButtonEvent);
    glfwSetScrollCallback(data->win, handleScrollEvent);
    glfwSetCursorPosCallback(data->win, handleCursorPosEvent);

    glfwSetWindowUserPointer(data->win, this);
}

Graphic::RenderSurface::~RenderSurface() noexcept { delete data; }

Graphic::RenderSurfacePrivate *
    Graphic::RenderSurface::create(std::string_view title, unsigned width, unsigned height) noexcept
{
    return new Graphic::RenderSurfacePrivate{ glfwCreateWindow(static_cast<int>(width), static_cast<int>(height),
                                                               title.data(), nullptr, nullptr) };
}

void Graphic::RenderSurface::show() noexcept
{
    int width, height;
    glfwGetWindowSize(data->win, &width, &height);
    CoreApplication::dispatchEvent(
        WindowResizeEvent{ this, static_cast<uint32_t>(width), static_cast<uint32_t>(height) });
    glfwShowWindow(data->win);
}

void Graphic::RenderSurface::hide() noexcept { glfwHideWindow(data->win); }

void Graphic::RenderSurface::resize(unsigned width, unsigned height) noexcept
{
    glfwSetWindowSize(data->win, static_cast<int>(width), static_cast<int>(height));
}

void Graphic::RenderSurface::swap() noexcept { glfwSwapBuffers(data->win); }

void Graphic::RenderSurface::make_current() noexcept { glfwMakeContextCurrent(data->win); }

void Graphic::RenderSurface::setWindowName(std::string_view title)
{
    fixe_string(title, [this](std::string_view titleFix) { glfwSetWindowTitle(data->win, titleFix.data()); });
}

glm::uvec2 Graphic::RenderSurface::getSize() noexcept
{
    static_assert(std::is_same_v<std::make_signed_t<glm::uvec2::value_type>, int>);
    static_assert(alignof(std::make_signed_t<glm::uvec2::value_type>) == alignof(int));
    glm::uvec2 size;
    glfwGetWindowSize(data->win, reinterpret_cast<int *>(&size.x), reinterpret_cast<int *>(&size.y));
    return size;
}
