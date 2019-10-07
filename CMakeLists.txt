cmake_minimum_required( VERSION 3.2.3 )

if( COMMAND cmake_policy )
    # Libraries linked via full path no longer produce linker search paths
	cmake_policy( SET CMP0003 NEW )
	# INTERFACE_LINK_LIBRARIES defines the link interface
	cmake_policy( SET CMP0022 NEW )
	# Disallow use of the LOCATION target property - so we set to OLD as we still need it
	cmake_policy( SET CMP0026 OLD )
	# MACOSX_RPATH is enabled by default
	cmake_policy( SET CMP0042 NEW )
    # Honor the visibility properties for SHARED target types only
    cmake_policy (SET CMP0063 OLD)
endif()

#set_property( GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake" )

set( CMAKE_BUILD_TYPE "Debug" CACHE STRING "Project build type" )
set( CMAKE_INSTALL_PREFIX "/usr/local/" CACHE PATH "Shmurho install prefix" )
set( CMAKE_VERBOSE_MAKEFILE ON )

if( NOT URHO3D_HOME )
	set( URHO3D_HOME $ENV{URHO3D_HOME} )
endif()
if( NOT URHO3D_HOME )
	message( FATAL_ERROR "URHO3D_HOME isnt set!" )
endif()
set( URHO3D_HOME ${URHO3D_HOME} CACHE PATH "Top directory containing Urho3D library and includes" )

message( STATUS "URHO3D_HOME: ${URHO3D_HOME}" )
list( APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/Modules" )
if( CMAKE_HOST_UNIX )
    list( APPEND CMAKE_MODULE_PATH "${URHO3D_HOME}/share/Urho3D/CMake/Modules" )
else()
    if( CMAKE_HOST_WIN32 )
        list( APPEND CMAKE_MODULE_PATH "${URHO3D_HOME}/share/CMake/Modules" )
    else()
        message( FATAL_ERROR "Unsupported host system: ${CMAKE_HOST_SYSTEM_NAME}" )
    endif()
endif()
message( STATUS "Current CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}" )

#
# Project
#
project( Shmurho CXX )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQURED ON )

option( SHMURHO_BUILD_DEMO "Build the demo game" ON )
find_package( Doxygen )
option( SHMURHO_BUILD_DOCS "Build the HTML-base documentation (requires Dixygen)" ${DOXYGEN_FOUND} )

include( UrhoCommon )
find_package( Urho3D MODULE REQUIRED )
include_directories( ${URHO3D_INCLUDE_DIRS} )

add_subdirectory( Shmurho/ )
if( SHMURHO_BUILD_DEMO )
       # add_subdirectory( Demo/ )
endif()
if( SHMURHO_BUILD_DOCS )
    add_subdirectory( Docs/ )
endif()

install(
	DIRECTORY ${CMAKE_SOURCE_DIR}/CMake/
	DESTINATION share/Shmurho/CMake/
)
install(
	FILES ${CMAKE_SOURCE_DIR}/README.md ${CMAKE_SOURCE_DIR}/LICENSE.md
	DESTINATION share/Shmurho/
)

