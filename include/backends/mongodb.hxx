//
// Created by amitprakash on 1/4/19.
//

#ifndef ROT_MONGODB_HXX
#define ROT_MONGODB_HXX

class MongoDBSingleton {
private:
    static MongoDBSingleton *instance;

    MongoDBSingleton() = default;

public:
    MongoDBSingleton(const MongoDBSingleton &) = delete;

    MongoDBSingleton &operator=(const MongoDBSingleton &) = delete;

    ~MongoDBSingleton();

    static MongoDBSingleton *getInstance();

    void init();
};

#endif //ROT_MONGODB_HXX
