FIND_PATH(Configuru_INCLUDE_DIR
        NAMES configuru.hpp
        PATHS ${CMAKE_INSTALL_INCLUDEDIR} "vendor"
        PATH_SUFFIXES Configuru)

IF(Configuru_INCLUDE_DIR)
    ADD_LIBRARY(Configuru INTERFACE IMPORTED GLOBAL)
    ADD_LIBRARY(configuru::Configuru ALIAS Configuru)
    SET_TARGET_PROPERTIES(Configuru PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES ${Configuru_INCLUDE_DIR})
ELSE()
    MESSAGE(FATAL_ERROR "Configuru not found")
ENDIF()