add_library(Core SHARED)
target_sources(Core PRIVATE
        core/Assert.hpp
        core/memory.hpp
        core/resource.hpp
        core/type_traits.hpp
        core/Layer.hpp
        core/Library.hpp
        core/Log.hpp
        core/Log.cpp
        core/Platform.cpp
        core/Platform.hpp
        core/functional.hpp
        core/utility.hpp
        core/utility.cpp)
target_include_directories(Core PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(Core PUBLIC spdlog::spdlog nlohmann_json::nlohmann_json)
target_compile_definitions(Core PUBLIC BM_SHARED_CORE PRIVATE BM_BUILD_CORE)
configure_binary_output(Core)
