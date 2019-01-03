//
// Created by amitprakash on 12/28/18.
//

#include <vector>

#include <producer/kinesisDataFirehoseProducer.hxx>
#include <utils/logger.hxx>

#include <aws/core/utils/Outcome.h>
#include <aws/firehose/FirehoseClient.h>
#include <aws/firehose/model/Record.h>
#include <aws/firehose/model/PutRecordBatchRequest.h>


KDFProducer::KDFProducer(std::shared_ptr<moodycamel::ConcurrentQueue<std::string> > &queue) : Producer(queue) {}

void KDFProducer::set_options(configuru::Config &config) {
    Producer::set_options(config);
    stream_name = config.get_or<std::string>("name", "kdf");
    client = std::make_shared<Aws::Firehose::FirehoseClient>(configuration);
}

std::shared_ptr<Aws::Client::AWSClient> KDFProducer::getClient() {
    return client;
}

void KDFResponseHandler(const Aws::Firehose::FirehoseClient *firehoseClient,
                        const Aws::Firehose::Model::PutRecordBatchRequest &request,
                        const Aws::Firehose::Model::PutRecordBatchOutcome &outcome,
                        const std::shared_ptr<const Aws::Client::AsyncCallerContext> &context) {
    auto logger = Logger::getLogger();

    std::string prefix;
    std::stringstream message;
    std::string output;
    spdlog::level::level_enum level;

    if (outcome.IsSuccess()) {
        prefix = "[+]";
        output = "Sucessfully pushed records";
        level = spdlog::level::debug;
    } else {
        prefix = "[-]";
        output = outcome.GetError().GetMessage();
        level = spdlog::level::err;
    }
    message << prefix << " <" << request.GetDeliveryStreamName() << ":" << context->GetUUID() << "> " << output;
    logger->log(level, firehoseClient->GetServiceClientName(), message.str().c_str());
}

void KDFProducer::async_push(std::string *data, std::size_t num_records) {
    Aws::Firehose::Model::PutRecordBatchRequest batchRequest;
    batchRequest.SetDeliveryStreamName(stream_name);
    std::vector<Aws::Firehose::Model::Record> records;

    for (std::size_t idx = 0; idx < num_records; ++idx) {
        std::string message{*(data + idx)};
        Aws::Firehose::Model::Record record;
        record.SetData(
                Aws::Utils::ByteBuffer(
                        (unsigned char *) message.c_str(),
                        message.length()));
        records.push_back(record);
    }
    batchRequest.SetRecords(records);
    client->PutRecordBatchAsync(batchRequest, KDFResponseHandler);
}

void KDFProducer::push(std::string *data, std::size_t num_records) {
    Aws::Firehose::Model::PutRecordBatchRequest batchRequest;
    batchRequest.SetDeliveryStreamName(stream_name);
    std::vector<Aws::Firehose::Model::Record> records;

    for (std::size_t idx = 0; idx < num_records; ++idx) {
        std::string message{*(data + idx)};
        Aws::Firehose::Model::Record record;
        record.SetData(
                Aws::Utils::ByteBuffer(
                        (unsigned char *) message.c_str(),
                        message.length()));
        records.push_back(record);
    }
    batchRequest.SetRecords(records);
    auto response = client->PutRecordBatch(batchRequest);
}