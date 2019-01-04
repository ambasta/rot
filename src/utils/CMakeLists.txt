PROJECT(logger)

ADD_LIBRARY(${PROJECT_NAME} logger.cxx)
ADD_LIBRARY(rot::logger ALIAS ${PROJECT_NAME})
TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        PUBLIC
            spdlog::spdlog
            configuru::Configuru
        PRIVATE
            rot::headers)