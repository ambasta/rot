//
// Created by amitprakash on 12/28/18.
//

#ifndef ROT_AWSSINGLETON_HXX
#define ROT_AWSSINGLETON_HXX

#include <aws/core/Aws.h>

class AWSSingleton {
private:
    static AWSSingleton *instance;
    Aws::SDKOptions sdkOptions;

    AWSSingleton() = default;
public:
    AWSSingleton(const AWSSingleton &) = delete;

    AWSSingleton &operator=(const AWSSingleton &) = delete;

    ~AWSSingleton();

    static AWSSingleton* getAWSInstance();

    void init();
};

#endif //ROT_AWSSINGLETON_HXX
