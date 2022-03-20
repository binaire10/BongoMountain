add_library(glfw_extension SHARED glfw_extension.cpp)
target_link_libraries(glfw_extension PRIVATE Graphic glad_loader private_opengl opengl_bootstrap glfw::glfw)

configure_plugin_output(glfw_extension)
