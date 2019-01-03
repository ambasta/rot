//
// Created by amitprakash on 12/28/18.
//

#ifndef ROT_KINESESDATAFIREHOSEPRODUCER_HXX
#define ROT_KINESESDATAFIREHOSEPRODUCER_HXX

#include <producer/producer.hxx>
#include <aws/firehose/FirehoseClient.h>

class KDFProducer : public Producer {
private:
    std::string stream_name;
    std::shared_ptr<Aws::Firehose::FirehoseClient> client = nullptr;

public:
    void set_options(configuru::Config&);

    std::shared_ptr<Aws::Client::AWSClient> getClient() override;

    void push(std::string[], std::size_t) override;

    void async_push(std::string[], std::size_t) override;

    KDFProducer(std::shared_ptr<moodycamel::ConcurrentQueue<std::string> > &);
};

#endif //ROT_KINESESDATAFIREHOSEPRODUCER_HXX
