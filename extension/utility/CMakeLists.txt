add_library(utility_extension SHARED utility_extension.cpp)
target_link_libraries(utility_extension PRIVATE Graphic stb::stb)

configure_plugin_output(utility_extension)
