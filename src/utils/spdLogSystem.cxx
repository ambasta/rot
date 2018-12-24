//
// Created by amitprakash on 12/24/18.
//

#include <unordered_map>
#include <utils/spdLogSystem.hxx>

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

static std::unordered_map<Aws::Utils::Logging::LogLevel, spdlog::level::level_enum> logLevelMapping{
        {Aws::Utils::Logging::LogLevel::Off,   spdlog::level::off},
        {Aws::Utils::Logging::LogLevel::Fatal, spdlog::level::critical},
        {Aws::Utils::Logging::LogLevel::Error, spdlog::level::err},
        {Aws::Utils::Logging::LogLevel::Warn,  spdlog::level::warn},
        {Aws::Utils::Logging::LogLevel::Info,  spdlog::level::info},
        {Aws::Utils::Logging::LogLevel::Debug, spdlog::level::debug},
        {Aws::Utils::Logging::LogLevel::Trace, spdlog::level::trace},
};

static std::unordered_map<std::string, Aws::Utils::Logging::LogLevel> logLevelStrMap{
        {"off", Aws::Utils::Logging::LogLevel::Off},
        {"critical", Aws::Utils::Logging::LogLevel::Fatal},
        {"error", Aws::Utils::Logging::LogLevel::Error},
        {"warn", Aws::Utils::Logging::LogLevel::Warn},
        {"info", Aws::Utils::Logging::LogLevel::Info},
        {"debug", Aws::Utils::Logging::LogLevel::Debug},
        {"trace", Aws::Utils::Logging::LogLevel::Trace}
};

void SpdLogSystem::ProcessFormattedStatement(Aws::String &&statement) {
    ProcessFormattedStatement(std::string(statement.data()));
}

void SpdLogSystem::ProcessFormattedStatement(std::string&& statement) {
    logger->log(logLevelMapping[GetLogLevel()], statement);
}

Logger::Logger(std::string levelStr) : Logger(logLevelStrMap[levelStr]) {}

Logger::Logger(Aws::Utils::Logging::LogLevel logLevel) : logLevel(logLevel) {
//    std::size_t filesize = 0, maxfiles = 0;
//    auto console_logger = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
//    auto filenam_logger = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("filename", "filepath", filesize, maxfiles);
//    spdlog::logger _logger("multi_sink", {console_logger, filenam_logger});
//    logger = spdlog::get("multi_sink");
//
//    logger = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
//
//    logger = std::make_shared<spdlog::logger>("loggerName", std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    std::initializer_list<std::shared_ptr<spdlog::sinks::sink> > log_sinks;
    std::string loggerName{"Hello"};
    logger = std::make_shared<spdlog::logger>(loggerName, log_sinks);
    spdlog::set_level(logLevelMapping[logLevel]);
    spdlog::set_pattern("[%h:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");
}

std::shared_ptr<SpdLogSystem> Logger::get_aws_logger() {
    return std::make_shared<SpdLogSystem>(Aws::Utils::Logging::LogLevel::Error, logger);
}

void Logger::trace(std::string &&statement) {
    log(spdlog::level::trace, statement);
}

void Logger::debug(std::string &&statement) {
    log(spdlog::level::debug, statement);
}

void Logger::fatal(std::string &&statement) {
    log(spdlog::level::critical, statement);
}

void Logger::error(std::string &&statement) {
    log(spdlog::level::err, statement);
}

void Logger::warn(std::string &&statement) {
    log(spdlog::level::warn, statement);
}

void Logger::info(std::string &&statement) {
    log(spdlog::level::info, statement);
}

void Logger::log(Aws::Utils::Logging::LogLevel level, std::string statement) {
    logger->log(logLevelMapping[level], statement);
}
void Logger::log(spdlog::level::level_enum level, std::string statement) {
    logger->log(level, statement);
}