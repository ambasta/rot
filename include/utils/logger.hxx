//
// Created by amitprakash on 12/24/18.
//

#ifndef ROT_LOGGER_H
#define ROT_LOGGER_H

#pragma once

#include <atomic>
#include <spdlog/spdlog.h>
#include <configuru.hpp>

#include <aws/core/utils/logging/LogLevel.h>
#include <aws/core/utils/logging/LogSystemInterface.h>

class Logger {
private:
    static Logger *instance;
    Aws::Utils::Logging::LogLevel awsLogLevel{Aws::Utils::Logging::LogLevel::Trace};
    std::shared_ptr<spdlog::logger> logger;

    Logger();

public:
    Logger(const Logger &) = delete;

    Logger &operator=(const Logger &) = delete;

    static Logger *getLogger();

    void init(configuru::Config &);

    Aws::Utils::Logging::LogLevel getAwsLogLevel() const;

    void log(spdlog::level::level_enum, const char *, const char *);
};

class spdlogForAWS : public Aws::Utils::Logging::LogSystemInterface {
public:
    spdlogForAWS() = default;

    ~spdlogForAWS() override = default;

    Aws::Utils::Logging::LogLevel GetLogLevel() const override;

    void Log(Aws::Utils::Logging::LogLevel, const char *, const char *, ...) override;

    void LogStream(Aws::Utils::Logging::LogLevel, const char *, const Aws::OStringStream &) override;
};

#endif //ROT_LOGGER_H
