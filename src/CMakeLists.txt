FIND_PACKAGE(ConcurrentQueue)
FIND_PACKAGE(Configuru)
FIND_PACKAGE(spdlog)

FIND_PACKAGE(Threads)

FIND_PACKAGE(CURL)
FIND_PACKAGE(AWSSDK REQUIRED COMPONENTS firehose)
FIND_PACKAGE(mongocxx REQUIRED)

ADD_SUBDIRECTORY(utils)
ADD_SUBDIRECTORY(backends)

ADD_LIBRARY(streamHandler streamHandler.cxx)
ADD_LIBRARY(rot::streamHandler ALIAS streamHandler)
TARGET_LINK_LIBRARIES(streamHandler
        PUBLIC
            MoodyCamel::ConcurrentQueue
            configuru::Configuru
        PRIVATE
            rot::headers)

ADD_SUBDIRECTORY(producer)

ADD_EXECUTABLE(${PROJECT_NAME} rot.cxx)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        PRIVATE
            rot::headers
            rot::backends
            rot::streamHandler
            rot::kdfProducer)