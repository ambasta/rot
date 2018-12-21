CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
SET(CMAKE_CXX_STANDARD 20)
INCLUDE(ExternalProject)
INCLUDE(GNUInstallDirs)

PROJECT(rot VERSION 0.1.0 LANGUAGES CXX)

# Verbose make
SET(CMAKE_VERBOSE_MAKEFILE ON)
SET(HEADER_INSTALL_DESTINATION "/usr/include")

# Disabled shared build
OPTION(BUILD_SHARED_LIBS "Request building of executable with shared libraries" ON)

MACRO(FIND_PACKAGE)
    IF(NOT "${ARG0}" IN_LIST AS_SUBPROJECT)
        _FIND_PACKAGE(${ARGV})
    ENDIF()
ENDMACRO()


FIND_PACKAGE(Git QUIET)
IF (GIT_FOUND AND EXISTS ${PROJECT_SOURCE_DIR}/.git)
    OPTION(GIT_SUBMODULE "Check submodules during build" ON)

    IF (GIT_SUBMODULE)
        MESSAGE("Updating submodules")
        EXECUTE_PROCESS(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMODULE_RESULT)

        IF (NOT GIT_SUBMODULE_RESULT EQUAL "0")
            MESSAGE(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMODULE_RESULT}")
        ENDIF ()
    ENDIF ()
ENDIF ()

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

FIND_PACKAGE(ConcurrentQueue)
FIND_PACKAGE(Configuru)
FIND_PACKAGE(spdlog)

IF (NOT spdlog_FOUND)
    ADD_SUBDIRECTORY(vendor/spdlog)
ENDIF()



#ADD_SUBDIRECTORY(cmake/concurrentqueue)
#ADD_SUBDIRECTORY(cmake/Configuru)

# FIND_PACKAGE(ConcurrentQueue)

#FIND_PACKAGE(spdlog QUIET)
#IF (NOT SPDLOG_FOUND)
#    IF (NOT EXISTS "${PROJECT_SOURCE_DIR}/third_party/spdlog/CMakeLists.txt")
#        MESSAGE(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
#    ELSE ()
#        ADD_SUBDIRECTORY(third_party/spdlog)
#    ENDIF ()
#ENDIF ()
#
#
#FIND_PATH(Configuru_INCLUDE_DIR "configuru.hpp" "third_party/Configuru")
#
#IF ((
#        NOT Configuru_INCLUDE_DIR) OR (
#        NOT EXISTS ${Configuru_INCLUDE_DIR}))
#    MESSAGE(
#            FATAL_ERROR
#            "The submodules were not downloaded!"
#            "GIT_SUBMODULE was turned off or failed."
#            "Please update submodules and try again.")
#ELSE ()
#    ADD_LIBRARY(Configuru INTERFACE)
#    TARGET_INCLUDE_DIRECTORIES(
#            Configuru INTERFACE
#            $<BUILD_INTERFACE:${Configuru_INCLUDE_DIR}>)
#ENDIF ()
#
#
#SET(LIBRARY_DIRS "")
#
#FIND_PACKAGE(Threads)
#LIST(APPEND LIBRARY_DIRS ${CMAKE_THREAD_LIBS_INIT})
#
#FIND_PACKAGE(CURL)
#LIST(APPEND INCLUDE_DIRS ${CURL_INCLUDE_DIRS})
#LIST(APPEND LIBRARY_DIRS ${CURL_LIBRARIES})
#
#IF (BUILD_SHARED_LIBS)
#    MESSAGE("Shared libs set to ON")
#    FIND_PACKAGE(AWSSDK REQUIRED COMPONENTS firehose)
#    FIND_PACKAGE(libmongocxx REQUIRED)
#
#    LIST(APPEND INCLUDE_DIRS ${LIBMONGOCXX_INCLUDE_DIRS})
#    LIST(APPEND INCLUDE_DIRS ${AWSSDK_INCLUDE_DIRS})
#
#    LIST(APPEND LIBRARY_DIRS ${LIBMONGOCXX_LIBRARIES})
#    LIST(APPEND LIBRARY_DIRS ${AWSSDK_LINK_LIBRARIES})
#ELSE ()
#    FIND_PACKAGE(libmongocxx-static)
#    FIND_PACKAGE(AWSSDK-static)
#
#    ADD_LIBRARY(AWSSDK STATIC IMPORTED)
#    ADD_LIBRARY(libmongocxx STATIC IMPORTED)
#
#    LIST(APPEND INCLUDE_DIRS ${ASKSDK_STATIC_INCLUDE_DIRS})
#    LIST(APPEND INCLUDE_DIRS ${LIBMONGOCXX_STATIC_INCLUDE_DIRS})
#
#    #    LIST(APPEND LIBRARY_DIRS AWSSDK)
#    #    LIST(APPEND LIBRARY )
#    #
#    #    SET(INCLUDE_DIRS ${LIBMONGOCXX_STATIC_INCLUDE_DIRS})
#    #    SET(LINK_LIBS ${LIBMONGOCXX_STATIC_LIBRARIES})
#ENDIF ()

ADD_EXECUTABLE(${PROJECT_NAME} main_temp.cxx)
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME}
        PUBLIC
            MoodyCamel::ConcurrentQueue
            configuru::Configuru
            spdlog::spdlog)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        MoodyCamel::ConcurrentQueue
        configuru::Configuru
        spdlog::spdlog)
#ADD_EXECUTABLE(
#        ${PROJECT_NAME}
#        main.cxx
#        producer.cxx
#        consumer.cxx
#        globals.hxx
#        producer.hxx)

# When headers are shared in both include (for global use) and src (for compilation purposes)
# For another, use local include dir while building, but install include dir on installataion
#TARGET_INCLUDE_DIRECTORIES(
#        ${PROJECT_NAME}
#        PUBLIC
#        ${ConcurrentQueue_INCLUDE_DIRS}
#        ${Configuru_INCLUDE_DIRS}
#        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#        $<INSTALL_INTERFACE:include>
#        PRIVATE
#        ${CMAKE_CURRENT_SOURCE_DIR}/src)
#
#TARGET_LINK_LIBRARIES(${PROJECT_NAME}
#        PRIVATE
#        ConcurrentQueue
#        Configuru)
#
#TARGET_LINK_LIBRARIES(
#        ${PROJECT_NAME}
#        ${LIBRARY_DIRS})


