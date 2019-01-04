PROJECT(backends)

ADD_LIBRARY(aws aws.cxx)
ADD_LIBRARY(${PROJECT_NAME}::aws ALIAS aws)
TARGET_LINK_LIBRARIES(aws
        PUBLIC
            aws-cpp-sdk-core
        PRIVATE
            rot::headers
            rot::logger)

ADD_LIBRARY(mongodb mongodb.cxx)
ADD_LIBRARY(${PROJECT_NAME}::mongodb ALIAS mongodb)
TARGET_LINK_LIBRARIES(mongodb
        PUBLIC
            mongo::mongocxx
        PRIVATE
            rot::headers)

ADD_LIBRARY(${PROJECT_NAME} backend.cxx)
ADD_LIBRARY(rot::${PROJECT_NAME} ALIAS backends)
TARGET_LINK_LIBRARIES(backends
        PUBLIC
            ${PROJECT_NAME}::aws
            ${PROJECT_NAME}::mongodb)
