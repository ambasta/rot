#include <iostream>
#include <thread>
#include <concurrentqueue.h>

#define CONFIGURU_IMPLEMENTATION 1
#include <configuru.hpp>

#include <AWSSetup.hxx>
#include <producer/kinesisDataFirehoseProducer.hxx>

int main() {

    auto initializeAWS = AWSSingleton::getAWSInstance();
    initializeAWS->init();

    auto queue = std::make_shared<moodycamel::ConcurrentQueue<std::string> >();

    auto config = configuru::parse_file("input.json", configuru::JSON);
    auto producer_config = config["producer"];

    auto producer = KDFProducer(queue);
    producer.set_options(producer_config);
    producer.run(true);

//
//    auto consumer = MongoConsumer(queue, consumer_config);
//    auto producer = KinesisProducer(queue, producer_config);
//
//    std::thread threads[2];
//    threads[0] = std::thread([&](){
//        producer.init();
//    });
//
//    threads[1] = std::thread([&](){
//        consumer.init();
//    });
//
//    for(auto& thread: threads) {
//        thread.join();
//    }

    return EXIT_SUCCESS;
}