//
// Created by amitprakash on 12/12/18.
//
#include "producer.hxx"
#include "globals.hxx"

#include <iostream>

#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/core/auth/AWSCredentialsProviderChain.h>

#include <aws/core/utils/logging/ConsoleLogSystem.h>
#include <aws/core/utils/Outcome.h>
#include <aws/core/utils/threading/Executor.h>

const Aws::Auth::AWSCredentials KinesisProducer::get_credentials(configuru::Config &config) {
    if (config.has_key("AWS_KEY_ID") && config.has_key("AWS_SECRET_KEY") && config.has_key("AWS_SESSION")) {
        return Aws::Auth::AWSCredentials(
                Aws::String((std::string)config["AWS_KEY_ID"]),
                Aws::String((std::string)config["AWS_SECRET_KEY"]),
                Aws::String((std::string)config["AWS_SESSION"]));
    }
    return Aws::Auth::DefaultAWSCredentialsProviderChain().GetAWSCredentials();
}

const Aws::Firehose::Model::Record KinesisProducer::make_record(const std::string_view data) {
    Aws::Firehose::Model::Record record;
    std::cout << "[KDF] Making record <" << data.data() << ">" << std::endl;
    auto buffer = Aws::Utils::ByteBuffer(reinterpret_cast<const unsigned char *>(data.data()), data.length());
    record.SetData(buffer);
    return record;
}

//const Aws::Firehose::Model::PutRecordRequest KinesisProducer::make_request(std::string_view data) {
//    Aws::Firehose::Model::PutRecordRequest request;
//    request.SetDeliveryStreamName(Aws::String(streamName));
//    request.SetRecord(make_record(data));
//    return request;
//}

const Aws::Firehose::Model::PutRecordBatchRequest KinesisProducer::make_batch_request(
        const std::string_view *data,
        std::size_t size) {
    Aws::Firehose::Model::PutRecordBatchRequest batchRequest;
    batchRequest.SetDeliveryStreamName(Aws::String(streamName));

    Aws::Vector<Aws::Firehose::Model::Record> records;

    for(int i = 0; i < size; ++i) {
        records.push_back(make_record(*(data + i)));
    }
    return batchRequest;
}

void KinesisProducer::push_data_to_stream(const std::string_view *data,
                                          std::size_t size,
                                          const Aws::Firehose::PutRecordBatchResponseReceivedHandler &callback) {
    auto request = make_batch_request(data, size);
    firehoseClient->PutRecordBatchAsync(request, callback);
}

void KinesisProducer::logger(const Aws::Firehose::FirehoseClient *firehoseClient,
                             const Aws::Firehose::Model::PutRecordBatchRequest &putRecordBatchRequest,
                             const Aws::Firehose::Model::PutRecordBatchOutcome &putRecordBatchOutcome,
                             const std::shared_ptr<const Aws::Client::AsyncCallerContext> &context) {
    putRecordBatchOutcome.IsSuccess() ?
    std::cout << "[+] Record successfully pushed to firehose stream" << std::endl :
    std::cerr << "[!] " << putRecordBatchOutcome.GetError().GetMessage() << std::endl;
}

void KinesisProducer::init() {
    while (pushToKDF) {
        std::string_view data[BATCH_SIZE];
        if (std::size_t size = queue.try_dequeue_bulk(data, BATCH_SIZE); size > 0) {
            std::cout << "Fetched " << size << " records. Attempting to push" << std::endl;
            push_data_to_stream(data, size, logger);
        }
    }
}

KinesisProducer::KinesisProducer(moodycamel::ConcurrentQueue<std::string_view>& queue, configuru::Config &config) : queue(queue) {
    options = Aws::SDKOptions{};
    options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Error;
    options.loggingOptions.logger_create_fn = [] {
        return std::make_shared<Aws::Utils::Logging::ConsoleLogSystem>(Aws::Utils::Logging::LogLevel::Error);
    };
    Aws::InitAPI(options);
    std::cout << "Initialized AWS API" << std::endl;

    if (config.has_key("STREAM_NAME")) {
        streamName = (std::string)config["STREAM_NAME"];
    }

    clientConfiguration = Aws::Client::ClientConfiguration{};
    clientConfiguration.executor = Aws::MakeShared<Aws::Utils::Threading::PooledThreadExecutor>(
            THREAD_POOL_NAME, MAX_CONCURRENCY);
    clientConfiguration.region = Aws::Region::AP_SOUTH_1;

    try {
        firehoseClient = Aws::MakeShared<Aws::Firehose::FirehoseClient>(
                ALLOCATION_TAG,
                get_credentials(config),
                clientConfiguration);
    } catch (std::exception& exception) {
        std::cerr << "Exception occurred while connecting to client: " << exception.what() << std::endl;
        throw exception;
    }
}

KinesisProducer::~KinesisProducer() {
    Aws::ShutdownAPI(options);
}