//
// Created by amitprakash on 1/4/19.
//

#include <backends/mongodb.hxx>

#include <mongocxx/instance.hpp>

MongoDBSingleton* MongoDBSingleton::instance = nullptr;

MongoDBSingleton* MongoDBSingleton::getInstance() {
    if (instance == nullptr) {
        instance = new MongoDBSingleton();
    }
    return instance;
}

void MongoDBSingleton::init() {
    mongocxx::instance _instance{};
}