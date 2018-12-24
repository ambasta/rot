PROJECT(logger)

ADD_LIBRARY(${PROJECT_NAME} spdLogSystem.cxx)
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        spdlog::spdlog)
