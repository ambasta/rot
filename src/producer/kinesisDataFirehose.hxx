//
// Created by amitprakash on 12/13/18.
//

#ifndef CDCREADER_KINESISDATAFIREHOSE_HXX
#define CDCREADER_KINESISDATAFIREHOSE_HXX

#include "producer.hxx"

#include <aws/firehose/FirehoseClient.h>
#include <aws/core/Aws.h>

class KinesisDataFirehoseProducer : public Producer<Aws::Firehose::FirehoseClient, Aws::SDKOptions> {
private:
    Aws::Client::ClientConfiguration clientConfiguration;
};

#endif //CDCREADER_KINESISDATAFIREHOSE_HXX
