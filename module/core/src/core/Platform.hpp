#ifndef BONGOMOUNTAIN_PLATFORM_HPP
#define BONGOMOUNTAIN_PLATFORM_HPP

#include <vector>
#include <memory>
#include <spdlog/spdlog.h>
#include <filesystem>
#include <nlohmann/json.hpp>

#include "config.hpp"
#include "Library.hpp"
#include "Layer.hpp"
#include "functional.hpp"
#include "DependencyManager.hpp"
#include "Assert.hpp"

namespace core
{
    template<typename CompareT = std::less<int>>
    struct CompareLayer
    {
        bool operator()(const auto &a, const auto &b)
        {
            return CompareT{}(a->getPriority(), b->getPriority());
        }
    };

    class BM_CORE_DCL ExtensionManager
    {
    public:
        ~ExtensionManager();
        void addLayer(std::shared_ptr<Layer> &&layer);

        void load(const std::filesystem::path &path_dir, const nlohmann::json &configs = {});

        void attach();

        void detach();

        void unload();

        void dispatchEvent(Event &event);

        [[nodiscard]] const std::vector<std::shared_ptr<Layer>> &getLayers() const { return m_layers; }

        template<typename T>
        std::shared_ptr<T> getSharedInstance() {
            return repository.createInstance<T>(storage);
        }

        template<typename T>
        std::unique_ptr<T> getUniqueInstance() {
            return repository.createInstance<T>(storage);
        }

        template<typename T>
        std::vector<std::unique_ptr<T>> geAllUniqueInstance() {
            return repository.createAllInstance<T>(storage);
        }

        template<typename T>
        std::vector<std::shared_ptr<T>> geAllSharedInstance() {
            return repository.createAllInstance<T>(storage);
        }

    private:
        bool                                m_isAttached = false;
        std::vector<std::shared_ptr<Layer>> m_layers;
        std::vector<Library>                m_libraries;
        RepositoryBindings repository;
        ObjectStorage storage;
    };

    class BM_CORE_DCL Platform : public ExtensionManager
    {
    public:
        Platform(int argc, const char **argv);

        const std::filesystem::path &getApplicationPath() { return m_executable; }

        static inline Platform &getInstance() {
            BM_CORE_ASSERT(instance);
            return *instance;
        }

    private:
        std::filesystem::path m_executable;
        nlohmann::json        m_config;
        static Platform *instance;
    };
}// namespace core

#endif// BONGOMOUNTAIN_PLATFORM_HPP
