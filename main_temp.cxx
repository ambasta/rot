//
// Created by amitprakash on 12/21/18.
//


#include <concurrentqueue.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

int main() {
    moodycamel::ConcurrentQueue<int> queue;
    queue.enqueue(10);
    auto console = spdlog::stdout_color_mt("console");
    console->info("Some log message");
}
