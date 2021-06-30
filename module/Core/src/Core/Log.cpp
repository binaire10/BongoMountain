#include "Assert.hpp"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

Log *Log::s_instance;

Log::Log() noexcept
{
    const std::initializer_list<spdlog::sink_ptr> logSinks{ std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
                                                            std::make_shared<spdlog::sinks::basic_file_sink_mt>(
                                                                "Engine.log", true) };
    (*std::next(logSinks.begin()))->set_level(spdlog::level::info);
    m_coreLogger.emplace("ENGINE", std::begin(logSinks), std::end(logSinks));
    m_appLogger.emplace("APP", std::begin(logSinks), std::end(logSinks));
    m_coreLogger->set_level(spdlog::level::trace);
    m_appLogger->set_level(spdlog::level::trace);
    BM_CORE_ASSERT(!s_instance, "only one instance of Log should exist !!");
    s_instance = this;
}
