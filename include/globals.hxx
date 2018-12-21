//
// Created by amitprakash on 12/13/18.
//

#ifndef CDCREADER_GLOBALS_HXX
#define CDCREADER_GLOBALS_HXX

#include <thread>

#define BATCH_SIZE 1000
#define MAX_CONCURRENCY std::thread::hardware_concurrency()
#define DATABASE "delhivery_db"
#define COLLECTION "packages"

#endif //CDCREADER_GLOBALS_HXX
