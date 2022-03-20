add_library(glad_loader STATIC)
target_sources(glad_loader PRIVATE glad_loader.cpp glad_loader.hpp)
target_link_libraries(glad_loader PUBLIC glad::glad Core)
target_include_directories(glad_loader PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)