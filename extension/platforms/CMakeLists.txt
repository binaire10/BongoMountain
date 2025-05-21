message("im gui directory : ${IMGUI_SRC_DIR}")

add_library(ImGuiGL3 OBJECT
        ${IMGUI_SRC_DIR}/imgui_impl_opengl3_loader.h
        ${IMGUI_SRC_DIR}/imgui_impl_opengl3.h
        ${IMGUI_SRC_DIR}/imgui_impl_opengl3.cpp)
target_include_directories(ImGuiGL3 PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/bindings>)
target_link_libraries(ImGuiGL3 PUBLIC imgui::imgui glad::glad)

add_library(ImGuiGLFW OBJECT
        ${IMGUI_SRC_DIR}/imgui_impl_glfw.h
        ${IMGUI_SRC_DIR}/imgui_impl_glfw.cpp)
target_link_libraries(ImGuiGLFW PUBLIC imgui::imgui glfw)
target_include_directories(ImGuiGLFW PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/bindings>)


add_subdirectory(glfw)
add_subdirectory(glad)
add_subdirectory(opengl)
