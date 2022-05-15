#include "Log.hpp"
#include "Assert.hpp"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "memory.hpp"

core::Log *core::Log::s_instance;

core::Log::Log() noexcept
{
    //    (*std::next(logSinks.begin()))->set_level(spdlog::level::info);

    m_coreLogger.emplace("ENGINE");
    m_appLogger.emplace("APP");
    configure_sink(*m_coreLogger);
    configure_sink(*m_appLogger);
    m_coreLogger->set_level(spdlog::level::trace);
    m_appLogger->set_level(spdlog::level::trace);
    s_instance = this;
}

core::Log::~Log() noexcept {
    s_instance = nullptr;
}

const std::shared_ptr<spdlog::logger> &core::Log::get(std::string_view name)
{
    auto iter = s_instance->m_custom.find(name);
    if(iter != s_instance->m_custom.end())
        return iter->second;

    auto        logger = fixe_string([](auto string_name) { return std::make_shared<spdlog::logger>(string_name.data());}, name);
    configure_sink(*logger);
    return s_instance->m_custom.emplace(name, std::move(logger) ).first->second;
}

void core::Log::configure_sink(spdlog::logger &logger)
{
    auto setup = [](auto a) {
        a->set_level(spdlog::level::trace);
        return a;
    };
    static const std::initializer_list<spdlog::sink_ptr> logSinks{
        std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
        setup(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Engine.log", true))
    };
    auto &sinks = logger.sinks();
    sinks.insert(sinks.end(), logSinks.begin(), logSinks.end());
}

void core::Log::set(const std::string &name, std::shared_ptr<spdlog::logger> logger)
{
    s_instance->m_custom.insert_or_assign(name, std::move(logger));
}
