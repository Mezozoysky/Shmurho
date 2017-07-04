# FindSUSA
# Module: locating SUSA library with cmake
# Author: Stanislav Demyanovich <mezozoysky@gmail.com>
#
# Usage:
#	set( ENV{SUSA_DIR} /path/to/susa )
#	find_package( SUSA REQUIRED )
#
# Defines:
# SUSA_INCLUDE_DIR	path to susa includes
# SUSA_FOUND		true if susa includes were found
#
# Note:  Works with static library only. No time for more.

set( SUSA_LOCATION_PREFIX_HINTS
    ${SUSA_DIR}
    $ENV{SUSA_DIR}
    /usr/local
)

if( NOT SUSA_LOCATION_PREFIX )
    find_path( SUSA_LOCATION_PREFIX
        NAMES include/susa/id.hpp
        HINTS ${SUSA_LOCATION_PREFIX_HINTS}
        DOC "Top-level directory, usually its /usr/local"
    )
endif()

if( NOT SUSA_LOCATION_PREFIX )
    if( SUSA_FIND_REQUIRED )
        message( FATAL_ERROR "Cant find SUSA location" )
    endif()
    if( NOT SUSA_FIND_QUIETLY )
        message( STATUS "Cant find SUSA location" )
    endif()
    return()
else()
    message( STATUS "Found SUSA location prefix: ${SUSA_LOCATION_PREFIX}" )
endif()

set( SUSA_INCLUDE_DIR ${SUSA_LOCATION_PREFIX}/include CACHE PATH "Path to SUSA includes" )
message( STATUS "Found SUSA include dir: ${SUSA_INCLUDE_DIR}" )

if( SUSA_INCLUDE_DIR )
    set( SUSA_FOUND TRUE )
endif()

