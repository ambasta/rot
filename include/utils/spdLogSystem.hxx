//
// Created by amitprakash on 12/24/18.
//

#ifndef ROT_SPLDLOGSYSTEM_H
#define ROT_SPLDLOGSYSTEM_H

#pragma once

#include <aws/core/Core_EXPORTS.h>
#include <aws/core/utils/logging/FormattedLogSystem.h>

#include <spdlog/spdlog.h>

#include <atomic>

class SpdLogSystem : public Aws::Utils::Logging::FormattedLogSystem {
private:
    std::shared_ptr<spdlog::logger> logger;

protected:
    void ProcessFormattedStatement(Aws::String&&) override;
    void ProcessFormattedStatement(std::string &&);

public:
    using Base = Aws::Utils::Logging::FormattedLogSystem;

    SpdLogSystem(Aws::Utils::Logging::LogLevel logLevel, std::shared_ptr<spdlog::logger> &logger) : Base(logLevel),
                                                                                                    logger(logger) {}

    ~SpdLogSystem() override {}
};

class Logger {
private:
    std::atomic<Aws::Utils::Logging::LogLevel> logLevel;
    std::shared_ptr<spdlog::logger> logger;
public:
    Logger(std::string);

    Logger(Aws::Utils::Logging::LogLevel);

    std::shared_ptr<SpdLogSystem> get_aws_logger();

    void error(std::string &&);

    void info(std::string &&);

    void debug(std::string &&);

    void fatal(std::string &&);

    void warn(std::string &&);

    void trace(std::string &&);

    void log(spdlog::level::level_enum, std::string);

    void log(Aws::Utils::Logging::LogLevel, std::string);
};

#endif //ROT_SPLDLOGSYSTEM_H
