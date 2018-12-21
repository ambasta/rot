//
// Created by amitprakash on 12/13/18.
//

#include <aws/core/utils/threading/Executor.h>
#include "kinesisDataFirehose.hxx"

KinesisDataFirehoseProducer::Producer() {
    thread_pool_name = std::string{};

    Aws::InitAPI();
    clientConfiguration = Aws::Client::ClientConfiguration{};
    clientConfiguration.executor = Aws::MakeShared<
            Aws::Utils::Threading::PooledThreadExecutor>(thread_pool_name, THREAD_POOL_SIZE);
    client = Aws::Firehose::FirehoseClient(options.get_credentials(), clientConfiguration);
}
