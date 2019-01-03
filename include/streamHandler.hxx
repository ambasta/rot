//
// Created by amitprakash on 12/28/18.
//

#ifndef ROT_STREAMHANDLER_HXX
#define ROT_STREAMHANDLER_HXX

#include <concurrentqueue.h>
#include <configuru.hpp>

class StreamHandler {
protected:
    std::shared_ptr<moodycamel::ConcurrentQueue<std::string> > &queue;
    std::size_t maximum_size;
public:
    StreamHandler(std::shared_ptr<moodycamel::ConcurrentQueue<std::string> > &);

    void set_options(configuru::Config &);
};

#endif //ROT_STREAMHANDLER_HXX
