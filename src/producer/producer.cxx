//
// Created by amitprakash on 12/28/18.
//

#include <producer/producer.hxx>
#include <aws/core/utils/threading/Executor.h>
#include <aws/firehose/model/PutRecordBatchRequest.h>
#include <aws/firehose/FirehoseClient.h>

Producer::Producer(std::shared_ptr<moodycamel::ConcurrentQueue<std::string> > &queue) : StreamHandler(queue) {
}

void Producer::set_options(configuru::Config &config) {
    StreamHandler::set_options(config);
    configuration.region = config.get_or<std::string>("region", "ap-southeast-1");
    configuration.maxConnections = config.get_or<unsigned>("maxConnections", 25);
    configuration.requestTimeoutMs = config.get_or<unsigned>("timeout", 1000);

    std::string pool_name = config.get_or<std::string>("name", "kdf");
    configuration.executor = std::make_shared<Aws::Utils::Threading::PooledThreadExecutor>(
            config.get_or<std::size_t>("max_threads", std::thread::hardware_concurrency()));
    stream_name = config.get_or<std::string>("name", "kdf");
}

void Producer::run(bool async) {
    while (stream) {
        std::string* data = new std::string[maximum_size];
        if (std::size_t num_records = queue->try_dequeue_bulk(data, maximum_size); num_records > 0) {
            if (async) {
                async_push(data, num_records);
            } else {
                push(data, num_records);
            }
        }
    }
}