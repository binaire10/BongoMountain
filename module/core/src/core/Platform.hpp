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

        void load(const std::filesystem::path &path, const nlohmann::json &configs);

        void attach();

        void detach();

        void unload();

        [[nodiscard]] const std::vector<std::shared_ptr<Layer>> &getLayers() const { return m_layers; }

    private:
        bool                                m_isAttached = false;
        std::vector<std::shared_ptr<Layer>> m_layers;
        std::vector<Library>                m_libraries;
    };

    class BM_CORE_DCL Platform : public ExtensionManager
    {
    public:
        Platform(int argc, const char **argv);

        void dispatchEvent(Event &event);

        [[nodiscard]] const nlohmann::json &getConfig() const { return m_config; }

        nlohmann::json &getConfig() { return m_config; }

    private:
        std::filesystem::path m_executable;
        nlohmann::json        m_config;
    };
}// namespace core

#endif// BONGOMOUNTAIN_PLATFORM_HPP
