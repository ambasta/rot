//
// Created by amitprakash on 12/28/18.
//

#ifndef ROT_PRODUCER_HXX
#define ROT_PRODUCER_HXX

#include <streamHandler.hxx>

#include <aws/core/client/AWSClient.h>
#include <aws/core/client/ClientConfiguration.h>

class Producer : public StreamHandler {
protected:
    Aws::Client::ClientConfiguration configuration;
    bool stream = true;
    std::string stream_name;

public:
    explicit Producer(std::shared_ptr<moodycamel::ConcurrentQueue<std::string> > &);

    virtual std::shared_ptr<Aws::Client::AWSClient> getClient() = 0;

    void set_options(configuru::Config &);

    virtual void push(std::string[], std::size_t) = 0;

    virtual void async_push(std::string[], std::size_t) = 0;

    void run(bool);
};

#endif //ROT_PRODUCER_HXX
