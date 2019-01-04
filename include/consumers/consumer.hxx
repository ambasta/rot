//
// Created by amitprakash on 1/4/19.
//

#ifndef ROT_CONSUMER_HXX
#define ROT_CONSUMER_HXX

#include <streamHandler.hxx>

class Consumer : public StreamHandler {
public:
    explicit Consumer(std::shared_ptr<moodycamel::ConcurrentQueue<std::string> > &);

    void set_options(configuru::Config &);
};

#endif //ROT_CONSUMER_HXX
