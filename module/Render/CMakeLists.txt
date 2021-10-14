add_library(BongoRender SHARED)
target_sources(BongoRender PRIVATE
        src/Render/ParticleManager.hpp
        src/Render/ParticleManager.cpp)
target_link_libraries(BongoRender PUBLIC BongoGraphic)
target_compile_definitions(BongoRender PUBLIC BM_SHARED_RENDER PRIVATE BM_BUILD_RENDER)
target_include_directories(BongoRender PUBLIC src)