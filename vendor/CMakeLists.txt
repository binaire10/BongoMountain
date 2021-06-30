add_subdirectory(glad)
add_subdirectory(glfw)

set(BUILD_SHARED_LIBS ON)
add_subdirectory(spdlog)

set(BUILD_SHARED_LIBS OFF)
add_subdirectory(OpenMesh-8.1)
add_subdirectory(stb)
add_subdirectory(glm)
