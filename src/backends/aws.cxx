//
// Created by amitprakash on 12/28/18.
//

#include <utils/logger.hxx>
#include <backends/aws.hxx>

AWSSingleton* AWSSingleton::instance = nullptr;

AWSSingleton* AWSSingleton::getInstance() {
    if (instance == nullptr) {
        instance = new AWSSingleton();
    }
    return instance;
}

AWSSingleton::~AWSSingleton() {
    Aws::ShutdownAPI(sdkOptions);
}

void AWSSingleton::init() {
    sdkOptions.loggingOptions.logger_create_fn = [] {
        return std::make_shared<spdlogForAWS>();
    };
    Aws::InitAPI(sdkOptions);
}