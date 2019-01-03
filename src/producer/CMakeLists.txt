PROJECT(kdfProducer)

ADD_LIBRARY(${PROJECT_NAME} producer.cxx kinesisDataFirehoseProducer.cxx)
ADD_LIBRARY(rot::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        PUBLIC
            MoodyCamel::ConcurrentQueue
            spdlog::spdlog
            configuru::Configuru
            aws-cpp-sdk-firehose)