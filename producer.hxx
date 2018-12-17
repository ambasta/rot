//
// Created by amitprakash on 12/13/18.
//

#ifndef CDCREADER_PRODUCER_HXX
#define CDCREADER_PRODUCER_HXX
//
// Created by amitprakash on 12/12/18.
//

#include <concurrentqueue.h>

#include <configuru.hpp>

#include <aws/core/Aws.h>
#include <aws/firehose/FirehoseClient.h>
#include <aws/firehose/model/Record.h>
#include <aws/firehose/model/PutRecordRequest.h>
#include <aws/firehose/model/PutRecordBatchRequest.h>

static const char* ALLOCATION_TAG = "FirehoseClient";

class KinesisProducer {
private:
    Aws::SDKOptions options;
    Aws::Client::ClientConfiguration clientConfiguration;
    std::shared_ptr<Aws::Firehose::FirehoseClient> firehoseClient;

    const char THREAD_POOL_NAME[20] = "kinesis_thread_pool";

    std::string streamName = "CDCDefault";
    moodycamel::ConcurrentQueue<std::string_view>& queue;
public:
    static const Aws::Auth::AWSCredentials get_credentials(configuru::Config &);

    static const Aws::Firehose::Model::Record make_record(
            std::string_view);

    const Aws::Firehose::Model::PutRecordRequest make_request(
            std::string_view);

    const Aws::Firehose::Model::PutRecordBatchRequest make_batch_request(
            const std::string_view*, std::size_t);

    void push_data_to_stream(
            const std::string_view*,
            std::size_t,
            const Aws::Firehose::PutRecordBatchResponseReceivedHandler&);

    static void logger(
            const Aws::Firehose::FirehoseClient*,
            const Aws::Firehose::Model::PutRecordBatchRequest&,
            const Aws::Firehose::Model::PutRecordBatchOutcome&,
            const std::shared_ptr<const Aws::Client::AsyncCallerContext>&
    );

    KinesisProducer(moodycamel::ConcurrentQueue<std::string_view>&, configuru::Config &);

    ~KinesisProducer();

    void init();
};

#endif //CDCREADER_PRODUCER_HXX
