#include "GraphicApplication.hpp"
#include <GLFW/glfw3.h>

void GraphicApplication::processEvent() noexcept { glfwPollEvents(); }
