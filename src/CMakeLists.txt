FIND_PACKAGE(ConcurrentQueue)
FIND_PACKAGE(Configuru)
FIND_PACKAGE(spdlog)

FIND_PACKAGE(Threads)

FIND_PACKAGE(CURL)
FIND_PACKAGE(AWSSDK REQUIRED COMPONENTS firehose)
FIND_PACKAGE(mongocxx REQUIRED)

get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

ADD_SUBDIRECTORY(utils)
ADD_SUBDIRECTORY(producer)

ADD_LIBRARY(streamHandler streamHandler.cxx)
ADD_LIBRARY(rot::streamHandler ALIAS streamHandler)

TARGET_INCLUDE_DIRECTORIES(streamHandler
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
TARGET_LINK_LIBRARIES(streamHandler
        PUBLIC
            MoodyCamel::ConcurrentQueue
            configuru::Configuru)

ADD_LIBRARY(awsSetup AWSSetup.cxx)
ADD_LIBRARY(rot::awsSetup ALIAS awsSetup)
TARGET_INCLUDE_DIRECTORIES(awsSetup
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
TARGET_LINK_LIBRARIES(awsSetup
        PUBLIC
            aws-cpp-sdk-core
        PRIVATE
            rot::logger)

ADD_EXECUTABLE(${PROJECT_NAME} rot.cxx)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        PRIVATE
            rot::awsSetup
            rot::streamHandler
            rot::kdfProducer)

#ADD_EXECUTABLE(${PROJECT_NAME} main.cxx)
#TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME}
#        PUBLIC
#            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
#            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
#
#TARGET_LINK_LIBRARIES(${PROJECT_NAME}
#        PUBLIC
#        MoodyCamel::ConcurrentQueue
#        configuru::Configuru
#        mongo::mongocxx
#        Threads::Threads
#        PRIVATE
#        rot::logger)