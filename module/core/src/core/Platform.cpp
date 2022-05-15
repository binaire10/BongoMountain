#include "Platform.hpp"
#include <iostream>
#include "Library.hpp"
#include "Assert.hpp"
#include <event/Event.hpp>

core::ExtensionManager::~ExtensionManager() { unload(); }

void core::ExtensionManager::load(const std::filesystem::path &path, const nlohmann::json &configs)
{
    BM_CORE_INFO("Load extensions in {}\nConfiguration : {}", path.string(), nlohmann::to_string(configs));
    for(auto file : std::filesystem::directory_iterator{ path })
    {
        auto path = file.path();
        if(path.extension() == ".dll")
        {
            BM_CORE_INFO("Load extension {}", path.filename().string());
            auto &library = m_libraries.emplace_back(path.string());
            auto  loader  = library.load<void(core::ExtensionManager &, const nlohmann::json &)>("initialize");
            if(!loader)
                BM_CORE_ERROR("Invalid loader !");
            else
                loader(*this, configs);
        }
    }

    m_isAttached = true;
}

void core::ExtensionManager::attach()
{
    BM_CORE_INFO("Attach Layers");
    std::sort(m_layers.begin(), m_layers.end(), CompareLayer<>{});

    std::for_each(m_layers.begin(), m_layers.end(), [](auto &module) { module->onAttach(); });
    m_isAttached = true;
}

void core::ExtensionManager::detach()
{
    BM_CORE_INFO("Detach Layers");
    std::for_each(m_layers.rbegin(), m_layers.rend(), [](auto &module) { module->onDetach(); });
    m_isAttached = false;
}

void core::ExtensionManager::unload()
{
    if(m_isAttached)
        detach();
    BM_CORE_INFO("Unload extensions");
    m_layers.clear();
    m_libraries.clear();
}

void core::ExtensionManager::addLayer(std::shared_ptr<Layer> &&layer)
{
    auto &m = m_layers.emplace_back(std::move(layer));
    if(m_isAttached)
        m->onAttach();

    if(!std::is_sorted(m_layers.begin(), m_layers.end(), CompareLayer<>{}))
        std::sort(m_layers.begin(), m_layers.end(), CompareLayer<>{});
}

void core::ExtensionManager::dispatchEvent(Event &event)
{
    for(auto &module : getLayers())
    {
        module->onEvent(event);
        if(event.isHandle())
            break;
    }
}
core::Platform::Platform(int argc, const char **argv)
{
    BM_CORE_INFO("Initialize Platform");
    BM_CORE_ASSERT(argc, "No argument to platform!");
    m_executable = argv[0];
}
