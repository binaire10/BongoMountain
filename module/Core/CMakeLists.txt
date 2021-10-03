project(Core)

add_library(BongoCore SHARED)
target_sources(BongoCore PRIVATE
        src/Event/Event.h
        src/Event/EventDispatcher.h
        src/Core/load_guard.hpp
        src/Core/CoreApplication.hpp
        src/Core/EventListener.hpp
        src/Core/CoreApplication.cpp
        src/Core/PointerLikeStackVariable.hpp
        src/Core/ModularModule.hpp
        src/Core/IModule.hpp
        src/Core/Assert.hpp
        src/Core/Log.hpp
        src/Core/Log.cpp
        src/Core/ressource.hpp
        src/Core/type_traits.hpp
        src/Core/memory.hpp
        src/Core/config.hpp)
target_include_directories(BongoCore PUBLIC src)
target_link_libraries(BongoCore PUBLIC common_options)
target_link_libraries(BongoCore PUBLIC spdlog::spdlog)
target_compile_definitions(BongoCore PRIVATE BM_BUILD_CORE)
target_compile_definitions(BongoCore PUBLIC BM_SHARED_CORE)

add_subdirectory(test)