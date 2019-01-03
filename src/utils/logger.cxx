//
// Created by amitprakash on 12/24/18.
//

#include <unordered_map>

#include <utils/logger.hxx>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>


std::unordered_map<std::string, const Aws::Utils::Logging::LogLevel> levelStrMapping{
        {"off", Aws::Utils::Logging::LogLevel::Off},
        {"fatal", Aws::Utils::Logging::LogLevel::Fatal},
        {"error", Aws::Utils::Logging::LogLevel::Error},
        {"warn", Aws::Utils::Logging::LogLevel::Warn},
        {"info", Aws::Utils::Logging::LogLevel::Info},
        {"debug", Aws::Utils::Logging::LogLevel::Debug},
        {"trace", Aws::Utils::Logging::LogLevel::Trace}
};

std::unordered_map<const Aws::Utils::Logging::LogLevel, const spdlog::level::level_enum> logLevelMapping{
        {Aws::Utils::Logging::LogLevel::Off,   spdlog::level::off},
        {Aws::Utils::Logging::LogLevel::Fatal, spdlog::level::critical},
        {Aws::Utils::Logging::LogLevel::Error, spdlog::level::err},
        {Aws::Utils::Logging::LogLevel::Warn,  spdlog::level::warn},
        {Aws::Utils::Logging::LogLevel::Info,  spdlog::level::info},
        {Aws::Utils::Logging::LogLevel::Debug, spdlog::level::debug},
        {Aws::Utils::Logging::LogLevel::Trace, spdlog::level::trace},
};

Logger* Logger::instance = nullptr;

Logger* Logger::getLogger() {
    if (instance == nullptr) {
        instance = new Logger();
    }
    return instance;
}

void Logger::init(configuru::Config& config) {
    auto level = config.get_or<std::string>("level", "trace");
    awsLogLevel = levelStrMapping.at(level);

    spdlog::set_level(logLevelMapping.at(awsLogLevel));

    auto pattern = config.get_or<std::string>("format", "*** [%l] [%H:%M:%S %z] [thread %t] %v ***");
    spdlog::set_pattern(pattern);

    std::vector<std::shared_ptr<spdlog::sinks::sink> > sinks;
    sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    if (config.has_key("path")) {
        sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(config.get<std::string>("path")));
    }

    logger = std::make_shared<spdlog::logger>("rot", std::begin(sinks), std::end(sinks));
}

void Logger::log(spdlog::level::level_enum level, const char *tag, const char *message) {
    logger->log(level, "[{}] {}", tag, message);
}

Logger::Logger() = default;

Aws::Utils::Logging::LogLevel Logger::getAwsLogLevel() const {
    return awsLogLevel;
}

Aws::Utils::Logging::LogLevel spdlogForAWS::GetLogLevel() const {
    return Logger::getLogger()->getAwsLogLevel();
}

void spdlogForAWS::Log(Aws::Utils::Logging::LogLevel level, const char *tag, const char *fmt, ...) {
    va_list args, tmp_args;
    va_start(args, fmt);
    va_copy(args, tmp_args);
    auto arglen = vsnprintf(nullptr, 0, fmt, tmp_args) + 1;
    va_end(tmp_args);
    char *buffer = new char[arglen];
    vsnprintf(buffer, (std::size_t)arglen, fmt, args);
    va_end(args);

    Logger::getLogger()->log(logLevelMapping.at(level), tag, buffer);
}

void spdlogForAWS::LogStream(Aws::Utils::Logging::LogLevel level, const char *tag, const Aws::OStringStream &stream) {
    Logger::getLogger()->log(logLevelMapping.at(level), tag, stream.str().data());
}