# FindShmurho
# Module:  locating Shmurho library with cmake
# Author:  Stanislav Demyanovich <mezozoysky@gmail.com>
# License: Zlib/libpng
#
# Usage:
#	set( ENV{SHMURHO_HOME} /path/to/shmurho )
#	find_package( Shmurho REQUIRED Phase Loader )
#
# Defines:
# Shmurho_INCLUDE_DIR   path to Shmurho includes
# Shmurho_LIBRARIES     list of found Shmurho libraries
# Shmurho_XXX_LIBRARY   found library file (XXX for component name: Phase, Loader, etc)
# Shmurho_FOUND         true if Shmurho components were found
#
# Note:  Works with static library only. No time for more.

set( Shmurho_LOCATION_PREFIX_HINTS
	${Shmurho_HOME}
	$ENV{SHMURHO_HOME}
    /usr/local
)

if( NOT Shmurho_LOCATION_PREFIX )
    find_path( Shmurho_LOCATION_PREFIX
        NAMES include/Shmurho/Phase/Switcher.hpp
        HINTS ${Shmurho_LOCATION_PREFIX_HINTS}
        DOC "Top-level directory, usually its /usr/local"
    )
endif()

if( NOT Shmurho_LOCATION_PREFIX )
    if( Shmurho_FIND_REQUIRED )
        message( FATAL_ERROR "Cant find Shmurho location" )
    endif()
    if( NOT Shmurho_FIND_QUIETLY )
        message( STATUS "Cant find Shmurho location" )
    endif()
    return()
else()
    message( STATUS "Found Shmurho location prefix: ${Shmurho_LOCATION_PREFIX}" )
endif()

if( NOT Shmurho_INCLUDE_DIR )
	set( Shmurho_INCLUDE_DIR ${Shmurho_LOCATION_PREFIX}/include CACHE PATH "Path to Shmurho includes" )
	message( STATUS "Found Shmurho include dir: ${Shmurho_INCLUDE_DIR}" )
endif()


foreach( component ${Shmurho_FIND_COMPONENTS} )
	if( NOT Shmurho_${component}_LIBRARY )

		#string(TOLOWER "${component}" component_lower )
		find_library(
			Shmurho_${component}_LIBRARY
			NAMES Shmurho${component}
			HINTS ${Shmurho_LOCATION_PREFIX}
			PATH_SUFFIXES
			lib
		)
		if ( NOT Shmurho_${component}_LIBRARY )
			if( Shmurho_FIND_REQUIRED )
				message( FATAL_ERROR "Cant find Shmurho${component} library" )
			endif()
			if( NOT Shmurho_FIND_QUIETLY )
				message( STATUS "Cant find Shmurho${component} library" )
			endif()
		else()
			message( STATUS "Found Shmurho${component} library: ${Shmurho_${component}_LIBRARY}" )
			list( APPEND Shmurho_LIBRARIES ${Shmurho_${component}_LIBRARY} )
			mark_as_advanced( Shmurho_${component}_LIBRARY )
			set( Shmurho_${component}_FOUND TRUE )
		endif()

	endif( NOT Shmurho_${component}_LIBRARY )
endforeach()



if( DEFINED Shmurho_LIBRARIES )
    set( Shmurho_FOUND TRUE )
	message( STATUS "Found Shmurho: ${Shmurho_LIBRARIES}" )
endif()

