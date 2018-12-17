//
// Created by amitprakash on 12/13/18.
//

#ifndef CDCREADER_PRODUCER_HXX
#define CDCREADER_PRODUCER_HXX

#include <thread>

#ifndef THREAD_POOL_SIZE
#define THREAD_POOL_SIZE std::thread::hardware_concurrency() + 1
#endif

#include <functional>
#include <string_view>
#include <vector>

template <typename Options>
class Config {
private:
    Options options;
public:
    Config();
    ~Config();

    auto get_credentials();
};

template <typename Client, typename Options>
class Producer {
private:
    std::string_view thread_pool_name;
    Client client;
    Config<Options> options;

public:
    Producer();

    ~Producer();

    const auto make_record(std::string_view);

    const auto make_request(std::string_view);

    const auto make_requests(std::vector<std::string_view>);

    const auto push(std::vector<std::string_view>);

    const auto async_push(std::vector<std::string_view>, std::function<void>);

    void init();
};

#endif //CDCREADER_PRODUCER_HXX
