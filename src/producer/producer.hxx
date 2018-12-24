//
// Created by amitprakash on 12/13/18.
//

#ifndef CDCREADER_PRODUCER_HXX
#define CDCREADER_PRODUCER_HXX

#include <string>
#include <string_view>
#include <vector>

#include <aws/firehose/FirehoseClient.h>
#include <aws/core/utils/logging/LogLevel.h>
#include <aws/core/utils/logging/FormattedLogSystem.h>
#include <aws/core/utils/logging/ConsoleLogSystem.h>

#include <concurrentqueue.h>
#include <configuru.hpp>
#include <spdlog.h>

#include <utils/spdLogSystem.hxx>

class AwsProducer {
protected:
    Aws::SDKOptions options{};
    std::shared_ptr<Aws::Client::ClientConfiguration> configuration = nullptr;
    std::shared_ptr<Aws::Client::AWSClient> client = nullptr;

    std::shared_ptr<moodycamel::ConcurrentQueue<std::string>>& queue;
    std::shared_ptr<Logger> logger = nullptr;

    virtual const auto make_record(std::string_view) = 0;
    virtual const auto make_request(std::string_view) = 0;
    virtual const auto make_request(std::vector<std::string_view>) = 0;
public:
    virtual AbstractProducer(moodycamel::ConcurrentQueue<std::string>&) = 0;
    virtual ~AbstractProducer(moodycamel::ConcurrentQueue<std::string>&) = 0;
    virtual void set_options(configuru::Config&, std::shared_ptr<Logger>) = 0;
    virtual const auto get_credentials() = 0;
    virtual const auto push(std::string_view) = 0;
    virtual const auto push(std::vector<std::string_view>) = 0;
    virtual const auto async_push(std::string_view) = 0;
    virtual const auto async_push(std::vector<std::string_view>) = 0;
    virtual void run() = 0;
};

class FirehoseProducer : public AwsProducer {
    void set_options(configuru::Config& config, std::shared_ptr<Logger> logger) override {
        auto logLevel = Aws::Utils::Logging::LogLevel::Error;

        if (config.has_key("logLevel")) {
            auto logLevelOption = config.get<std::string>('logLevel');
            std::unordered_map<std::string, enum> logLevelMapping{
                    {"off", Aws::Utils::Logging::LogLevel::Off},
                    {"info", Aws::Utils::Logging::LogLevel::Fatal},
                    {"error", Aws::Utils::Logging::LogLevel::Error},
                    {"warn", Aws::Utils::Logging::LogLevel::Warn},
                    {"info", Aws::Utils::Logging::LogLevel::Info},
                    {"debug", Aws::Utils::Logging::LogLevel::Debug},
                    {"trace", Aws::Utils::Logging::LogLevel::Trace},
            };

            if (logLevelMapping.contains(config.get<std::string>('logLevel'))) {
                logLevel = logLevelMapping[config.get<std::string>('logLevel')];
            }
        }
        options.loggingOptions.logLevel = logLevel;
        options.loggingOptions.logger_create_fn = logger.get_aws_logger();
    }

    void setLogging() {
        Aws::Utils::Logging::InitializeAWSLogging(
                std::make_shared<Aws::Utils::Logging::DefaultLogSystem>(
                        "Firehose", logLevel, "aws_sdk_"));
    }

    FirehoseProducer()

//    FirehoseProducer() {
//        /*
//         * For implicit client credentials
//         * Aws::MakeShared<Aws::Firehose::FirehoseClient>(ALLOCATION_TAG, config);
//         */
////        client = Aws::MakeShared<Aws::Firehose::FirehoseClient>(
////                ALLOCATION_TAG,
////                get_credentials(),
////                config);
////        Since CUSTOM_MEMORY_MANAGEMENT is off, Aws::Types resolve to std::Types
//        client = std::make_shared<Aws::Firehose::FirehoseClient>(get_credentials(), config);
//    }
};
#endif //CDCREADER_PRODUCER_HXX
