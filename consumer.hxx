//
// Created by amitprakash on 12/14/18.
//

#ifndef CDCREADER_CONSUMER_HXX
#define CDCREADER_CONSUMER_HXX

#include <concurrentqueue.h>

#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>

#include <configuru.hpp>

class MongoClientWrapper {
private:
    std::unique_ptr<mongocxx::instance> _instance = nullptr;
    std::unique_ptr<mongocxx::pool> _pool = nullptr;

    MongoClientWrapper() = default;

public:

    void configure(std::unique_ptr<mongocxx::instance>, std::unique_ptr<mongocxx::pool>);

    mongocxx::pool::entry acquire();

//    bsoncxx::stdx::optional<mongocxx::pool::entry> try_acquire();

    static MongoClientWrapper &instance();
};

class MongoConsumer {
private:
    int checkpoint = 0;
    moodycamel::ConcurrentQueue<std::string>& queue;
public:
    auto get_server_status();

    MongoConsumer(moodycamel::ConcurrentQueue<std::string>&, configuru::Config&);

    void init();
};

#endif //CDCREADER_CONSUMER_HXX
