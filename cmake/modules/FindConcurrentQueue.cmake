FIND_PATH(ConcurrentQueue_INCLUDE_DIR
        NAMES concurrentqueue.h
        PATHS ${CMAKE_INSTALL_INCLUDEDIR} "vendor"
        PATH_SUFFIXES concurrentqueue)

IF(ConcurrentQueue_INCLUDE_DIR)
    ADD_LIBRARY(ConcurrentQueue INTERFACE IMPORTED GLOBAL)
    ADD_LIBRARY(MoodyCamel::ConcurrentQueue ALIAS ConcurrentQueue)
    SET_TARGET_PROPERTIES(ConcurrentQueue PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${ConcurrentQueue_INCLUDE_DIR})
ELSE()
    MESSAGE(FATAL_ERROR "ConcurrentQueue not found")
ENDIF()