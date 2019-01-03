PROJECT(logger)

ADD_LIBRARY(${PROJECT_NAME} logger.cxx)
ADD_LIBRARY(rot::logger ALIAS ${PROJECT_NAME})

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME}
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        PUBLIC
            spdlog::spdlog
            configuru::Configuru)