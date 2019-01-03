//
// Created by amitprakash on 12/28/18.
//

#include <streamHandler.hxx>

StreamHandler::StreamHandler(std::shared_ptr<moodycamel::ConcurrentQueue<std::string> > &queue) : queue(queue) {}

void StreamHandler::set_options(configuru::Config &config) {
    maximum_size = config.get_or<std::size_t>("batch_size", 1000);
}