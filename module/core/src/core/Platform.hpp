#ifndef BONGOMOUNTAIN_PLATFORM_HPP
#define BONGOMOUNTAIN_PLATFORM_HPP

#include <vector>
#include <memory>
#include <spdlog/spdlog.h>
#include <filesystem>
#include <nlohmann/json.hpp>

#include "config.hpp"
#include "Module.hpp"
#include "Layer.hpp"
#include "functional.hpp"

namespace core
{
    template<typename CompareT = std::less<int>>
    struct CompareLayer
    {
        bool operator()(const std::shared_ptr<Layer> &a, const std::shared_ptr<Layer> &b)
        {
            return CompareT{}(a->getPriority(), b->getPriority());
        }
    };

    class BM_CORE_DCL ExtensionManager
    {
    public:
        ~ExtensionManager();
        void addLayer(std::shared_ptr<Layer> &&layer);

        void load(const std::filesystem::path &path, const nlohmann::json &configs = {});

        void attach();

        void detach();

        void unload();

        void dispatchEvent(Event &event);

        [[nodiscard]] const std::vector<std::shared_ptr<Layer>> &getLayers() const { return m_layers; }

    private:
        bool                                m_isAttached = false;
        std::vector<std::shared_ptr<Layer>> m_layers;
        std::vector<Module>                 m_modules;
    };

    class BM_CORE_DCL Platform : public ExtensionManager
    {
    public:
        Platform(int argc, const char **argv);

        const std::filesystem::path &getApplicationPath() { return m_executable; }

        static Platform &getInstance() {
            return *instance;
        }

    private:
        std::filesystem::path m_executable;

        static Platform *instance;
    };
}// namespace core

#endif// BONGOMOUNTAIN_PLATFORM_HPP
