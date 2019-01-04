# rot
A general purpose data replicator. This project allows users to replicate data from one database to another.

Following sources and sinks are supported:

    * MongoDB: Via change streams
    * Postgres: Via replica
Following sinks are supported:

    * KinesisDataFirehose
    * Kinesis [ToDo]
    * Kafka [ToDo]


Currently the project only pushes live deltas from the source.

## Dependencies
The project aims to be all inclusive packaging third party libraries in src/vendor. However, system-wide dependencies are defaulted to for:

    * mongo-cxx-driver
    * aws-cpp-sdk[core,firehose]
    * spdlog
    * concurrentqueue