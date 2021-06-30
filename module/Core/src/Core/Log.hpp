#ifndef GAMEENGINEBONGO_LOG_HPP
#define GAMEENGINEBONGO_LOG_HPP

#include "config.hpp"
#include <memory>
#include <optional>
#include <spdlog/spdlog.h>

class BM_CORE_DCL Log
{
public:
    Log(const Log &) = delete;
    Log &operator=(const Log &) = delete;
    Log() noexcept;
    virtual ~Log() noexcept = default;

    static inline std::optional<spdlog::logger> &coreLogger() noexcept { return s_instance->m_coreLogger; }

    static inline std::optional<spdlog::logger> &appLogger() noexcept { return s_instance->m_appLogger; }

private:
    static Log *                  s_instance;
    std::optional<spdlog::logger> m_coreLogger = std::nullopt;
    std::optional<spdlog::logger> m_appLogger  = std::nullopt;
};

#define BM_ERROR   Log::appLogger()->error
#define BM_WARNING Log::appLogger()->warn
#define BM_TRACE   Log::appLogger()->trace
#define BM_INFO    Log::appLogger()->info
#define BM_DEBUG   Log::appLogger()->debug

#define BM_CORE_ERROR   Log::coreLogger()->error
#define BM_CORE_WARNING Log::coreLogger()->warn
#define BM_CORE_TRACE   Log::coreLogger()->trace
#define BM_CORE_INFO    Log::coreLogger()->info
#define BM_CORE_DEBUG   Log::coreLogger()->debug

#endif// GAMEENGINEBONGO_LOG_HPP
