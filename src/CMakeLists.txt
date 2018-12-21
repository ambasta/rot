FIND_PACKAGE(ConcurrentQueue)
FIND_PACKAGE(Configuru QUIET)

FIND_PACKAGE(spdlog QUIET)
IF (NOT spdlog_FOUND)
    ADD_SUBDIRECTORY(vendor/spdlog)
ENDIF()

FIND_PACKAGE(Threads)

FIND_PACKAGE(CURL)
FIND_PACKAGE(AWSSDK REQUIRED COMPONENTS firehose)
FIND_PACKAGE(libmongocxx REQUIRED)

ADD_SUBDIRECTORY(consumer)
ADD_SUBDIRECTORY(producer)

ADD_EXECUTABLE(${PROJECT_NAME} main.cxx)
TARGET_INCLUDE_DIRECTORIES(
        ${PROJECT_NAME}
        PUBLIC
        MoodyCamel::ConcurrentQueue
        configuru::Configuru
        spdlog::spdlog
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        PUBLIC
        MoodyCamel::ConcurrentQueue
        configuru::Configuru
        spdlog::spdlog
        PRIVATE
        rot::producer
        rot::consumer)