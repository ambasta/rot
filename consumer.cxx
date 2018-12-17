//
// Created by amitprakash on 12/13/18.
//
#include "globals.hxx"
#include "consumer.hxx"

#include <iostream>

#include <curl/curl.h>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/stdx/make_unique.hpp>
#include <bsoncxx/string/to_string.hpp>

#include <mongocxx/options/change_stream.hpp>
#include <mongocxx/client.hpp>
#include <sstream>


std::string_view encode(std::string_view source, size_t length) {
    auto *curl = curl_easy_init();

    if (curl) {
        char *output = curl_easy_escape(curl, source.data(), length);

        if (output) {
            return output;
        }
    }
    return nullptr;
}


void MongoClientWrapper::configure(std::unique_ptr<mongocxx::instance> instance, std::unique_ptr<mongocxx::pool> pool) {
    _instance = std::move(instance);
    _pool = std::move(pool);
}

mongocxx::pool::entry MongoClientWrapper::acquire() {
    return _pool->acquire();
}

bsoncxx::stdx::optional<mongocxx::pool::entry> MongoClientWrapper::try_acquire() {
    return _pool->try_acquire();
}

MongoClientWrapper &MongoClientWrapper::instance() {
    static MongoClientWrapper instance;
    return instance;
}

auto MongoConsumer::get_server_status() {
    try {
        auto entry = MongoClientWrapper::instance().acquire();
        bsoncxx::builder::basic::document query{};
        query.append(bsoncxx::builder::basic::kvp("serverStatus", 1));
        auto output = (*entry)["test"].run_command(query.extract());
        return bsoncxx::string::to_string(output.view()["version"].get_utf8().value);
    } catch (const std::exception &exception) {
        std::cerr << "Unexpected exception: " << exception.what() << std::endl;
        throw exception;
    }
}

MongoConsumer::MongoConsumer(moodycamel::ConcurrentQueue<std::string_view> &queue, configuru::Config &config) : queue(
        queue) {

    std::stringstream uri_string;

    uri_string << "mongodb://";

    if (config.has_key("user") and config.has_key("pass")) {
        std::string user = (std::string)config["user"];
        std::string pass = (std::string)config["pass"];
        uri_string << encode(user, user.length()) << ":" << encode(pass, pass.length()) << "@";
    }

    if (config.has_key("hosts")) {
        int32_t port = 27017;
        int prior = 0;

        if (config.has_key("port")) {
            port = (int32_t) config["port"];
        }

        for (auto host: config["hosts"].as_array()) {

            if (prior > 0) {
                uri_string << ",";
            }

            if (config.has_key("port")) {
                port = (int32_t) config["port"];
            }
            uri_string << (std::string)host["host"] << ":" << port;
            prior++;
        }
    } else {
        uri_string << "127.0.0.1:27017";
    }
    uri_string << "/";

    if (config.has_key("authSource")) {
        uri_string << "?authSource=" << (std::string)config["authSource"];
    }

    std::cout << "URI: " << uri_string.str() << std::endl;

    mongocxx::uri uri{uri_string.str()};
    auto instance = bsoncxx::stdx::make_unique<mongocxx::instance>();
    MongoClientWrapper::instance().configure(
            std::move(instance),
            bsoncxx::stdx::make_unique<mongocxx::pool>(std::move(uri)));

    if (get_server_status() < "3.6") {
        std::cerr << "[MongoDB]Version 3.6 or greater required" << std::endl;
    }
}

void MongoConsumer::init() {
    mongocxx::options::change_stream change_stream_options;
    change_stream_options.max_await_time(std::chrono::milliseconds{1000});

    if (checkpoint) {
        bsoncxx::builder::basic::document document{};
        document.append(bsoncxx::builder::basic::kvp("_id", checkpoint));
        change_stream_options.resume_after(document.view());
    }
    change_stream_options.batch_size(BATCH_SIZE);
    auto entry = MongoClientWrapper::instance().acquire();
    auto stream = (*entry)[DATABASE][COLLECTION].watch(change_stream_options);

    while (true) {
        for (const auto &event: stream) {
            auto event_json_string = bsoncxx::to_json(event);
            queue.enqueue(event_json_string);
        }
    }
}