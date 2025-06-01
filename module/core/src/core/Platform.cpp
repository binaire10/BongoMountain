#include "Platform.hpp"
#include <iostream>
#include "Assert.hpp"
#include <event/Event.hpp>

using namespace std::string_view_literals;

#ifndef EXTENSIONS_SUFFIX
#    if defined(_WIN32)
#        define EXTENSIONS_SUFFIX ".dll"sv
#    elif defined(_linux) || defined(linux)
#        define EXTENSIONS_SUFFIX ".so"sv
#    endif
#endif

core::Platform::Platform(int argc, const char **argv)
{
    BM_CORE_INFO("Initialize Platform");
    BM_CORE_ASSERT(argc, "No argument to platform!");
    m_executable = computeApplicationPath();
    if(m_executable.empty() && argc > 0)
        m_executable = argv[0];
    instance = this;
}

core::ExtensionManager::~ExtensionManager() { unload(); }

void core::ExtensionManager::load(const std::filesystem::path &path_dir, const nlohmann::json &configs)
{
    BM_CORE_INFO("Load extensions in {}\nConfiguration : {}", path_dir.string(), nlohmann::to_string(configs));
    RepositoryBindings repos;
    for(const auto &file : std::filesystem::directory_iterator{ path_dir })
    {
        auto &path = file.path();
        if(path.extension() == EXTENSIONS_SUFFIX)
        {
            BM_CORE_INFO("Load extension {}", path.filename().string());
            auto &library = m_libraries.emplace_back(path);
            auto  loader  = library.load<void(core::RepositoryBindings &, const nlohmann::json &)>("initialize");
            if(!loader)
                BM_CORE_ERROR("Invalid loader !");
            else
                loader(repos, configs);
        }
    }
    auto layers = repos.createAllInstance<Layer>(storage);
    if(m_isAttached)
    {
        std::sort(layers.begin(), layers.end(), CompareLayer<>{});

        std::for_each(layers.begin(), layers.end(), [](auto &module) { module->onAttach(); });
    }
    m_layers.insert(m_layers.end(), std::make_move_iterator(layers.begin()), std::make_move_iterator(layers.end()));
    repository = std::move(repos);
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
    storage.clear();
    repository.clear();
    m_libraries.clear();
}

void core::ExtensionManager::addLayer(std::shared_ptr<Layer> &&layer)
{
    auto &m = m_layers.emplace_back(std::move(layer));
    if(m_isAttached)
        m->onAttach();
    else if(!std::is_sorted(m_layers.begin(), m_layers.end(), CompareLayer<>{}))
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

void core::ExtensionManager::compute()
{
    std::for_each(m_layers.begin(), m_layers.end(), [](auto &module) { module->onBegin(); });
    std::for_each(m_layers.begin(), m_layers.end(), [](auto &module) { module->onUpdate(); });
    std::for_each(m_layers.rbegin(), m_layers.rend(), [](auto &module) { module->onEnd(); });
}

std::filesystem::path core::Platform::computeApplicationPath()
{
#if defined(_WIN32) or defined(WIN32)
    std::string filebuffer;
    const auto  initialSize = filebuffer.capacity();
    filebuffer.resize(initialSize > 0 ? initialSize : MAX_PATH);
    DWORD result = 0;
    do {
        if(result != 0)
        {
            filebuffer.resize(filebuffer.size() * 2);
        }
        result = GetModuleFileNameA(nullptr, filebuffer.data(), filebuffer.size());
    } while(result == filebuffer.size() && GetLastError() == ERROR_INSUFFICIENT_BUFFER);
    filebuffer.resize(result);
    return filebuffer;
#elif defined(_linux) or defined(linux)
    return std::filesystem::canonical("/proc/self/exe");
#else
    return {};
#endif
}

core::Platform *core::Platform::instance = nullptr;

void core::Platform::exec()
{
    running.store(true, std::memory_order_release);
    while(running.load(std::memory_order_acquire)) { compute(); }
}

void core::Platform::exit() { running.store(false, std::memory_order_release); }
