FIND_PACKAGE(ConcurrentQueue)
FIND_PACKAGE(Configuru)

FIND_PACKAGE(spdlog)
IF (NOT spdlog_FOUND)
    MESSAGE("Spdlog not found")
    ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/src/vendor/spdlog)
ELSE()
    MESSAGE("Spdlog found")
ENDIF()

FIND_PACKAGE(Threads)

FIND_PACKAGE(CURL)
FIND_PACKAGE(AWSSDK REQUIRED COMPONENTS firehose)
FIND_PACKAGE(libmongocxx REQUIRED)

# ADD_SUBDIRECTORY(utils)
ADD_SUBDIRECTORY(consumer)
ADD_SUBDIRECTORY(producer)

ADD_LIBRARY(logger utils/spdLogSystem.cxx)
ADD_LIBRARY(rot::logger ALIAS logger)
TARGET_INCLUDE_DIRECTORIES(logger
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
TARGET_LINK_LIBRARIES(logger
        PUBLIC
        spdlog::spdlog)

ADD_EXECUTABLE(${PROJECT_NAME} main.cxx)
TARGET_INCLUDE_DIRECTORIES(
        ${PROJECT_NAME}
        PUBLIC
        MoodyCamel::ConcurrentQueue
        configuru::Configuru
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        PUBLIC
        MoodyCamel::ConcurrentQueue
        configuru::Configuru
        PRIVATE
        rot::logger
        rot::producer
        rot::consumer)