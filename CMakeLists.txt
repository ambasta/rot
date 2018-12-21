CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
SET(CMAKE_CXX_STANDARD 20)
INCLUDE(ExternalProject)
INCLUDE(GNUInstallDirs)

PROJECT(rot VERSION 0.1.0 LANGUAGES CXX)

# Verbose make
SET(CMAKE_VERBOSE_MAKEFILE ON)
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
ADD_SUBDIRECTORY(src)
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC include)
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


