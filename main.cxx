#include "globals.hxx"
#include "consumer.hxx"
#include "producer.hxx"

#include <iostream>
#include <thread>

#include <concurrentqueue.h>

#define CONFIGURU_IMPLEMENTATION 1

#include <configuru.hpp>


int main() {
    moodycamel::ConcurrentQueue<std::string_view> queue;

    auto config = configuru::parse_file("input.json", configuru::JSON);
    auto consumer_config = config["consumer"];
    auto producer_config = config["producer"];

    auto consumer = MongoConsumer(queue, consumer_config);
    auto producer = KinesisProducer(queue, producer_config);

    std::thread threads[2];
    threads[0] = std::thread([&](){
        producer.init();
    });

    threads[1] = std::thread([&](){
        consumer.init();
    });

    for(auto& thread: threads) {
        thread.join();
    }

    return EXIT_SUCCESS;
}