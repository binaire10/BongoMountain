#ifndef BONGOMOUNTAIN_LOG_HPP
#define BONGOMOUNTAIN_LOG_HPP

#include "config.hpp"
#include <memory>
#include <map>
#include <optional>
#include <spdlog/spdlog.h>

namespace core
{
    class BM_CORE_DCL Log
    {
    public:
        Log(const Log &)            = delete;
        Log &operator=(const Log &) = delete;
        Log() noexcept;
        ~Log() noexcept;

        static inline std::optional<spdlog::logger> &coreLogger() noexcept { return s_instance->m_coreLogger; }
        static inline std::optional<spdlog::logger> &appLogger() noexcept { return s_instance->m_appLogger; }

        static const std::shared_ptr<spdlog::logger> &get(std::string_view name);
        static void set(const std::string &name, std::shared_ptr<spdlog::logger> logger);

    private:
        static void configure_sink(spdlog::logger &logger);

        static Log *s_instance;

        std::optional<spdlog::logger> m_coreLogger = std::nullopt;
        std::optional<spdlog::logger> m_appLogger  = std::nullopt;

        std::map<std::string, std::shared_ptr<spdlog::logger>, std::less<>> m_custom;
    };
}// namespace core

#define BM_ERROR    core::Log::appLogger()->error
#define BM_WARNING  core::Log::appLogger()->warn
#define BM_CRITICAL core::Log::appLogger()->critical
#define BM_TRACE    core::Log::appLogger()->trace
#define BM_INFO     core::Log::appLogger()->info
#define BM_DEBUG    core::Log::appLogger()->debug

#define BM_CORE_ERROR    core::Log::coreLogger()->error
#define BM_CORE_WARNING  core::Log::coreLogger()->warn
#define BM_CORE_CRITICAL core::Log::coreLogger()->critical
#define BM_CORE_TRACE    core::Log::coreLogger()->trace
#define BM_CORE_INFO     core::Log::coreLogger()->info
#define BM_CORE_DEBUG    core::Log::coreLogger()->debug

#endif// BONGOMOUNTAIN_LOG_HPP
