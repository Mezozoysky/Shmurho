cmake_minimum_required( VERSION 3.5.2 )

if( COMMAND cmake_policy )
	cmake_policy( SET CMP0003 NEW )
	# INTERFACE_LINK_LIBRARIES defines the link interface
	cmake_policy( SET CMP0022 NEW )
	# Disallow use of the LOCATION target property - so we set to OLD as we still need it
	cmake_policy( SET CMP0026 OLD )
	# MACOSX_RPATH is enabled by default
	cmake_policy( SET CMP0042 NEW )
endif()

set( CMAKE_BUILD_TYPE "Debug" CACHE STRING "Project build type" )
set( CMAKE_INSTALL_PREFIX "../install" CACHE PATH "Install prefix" )
set( EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin" )
set( LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib" )
set( CMAKE_MACOSX_RPATH ON )
set( CMAKE_VERBOSE_MAKEFILE ON )

set_property( GLOBAL PROPERTY USE_FOLDERS ON )
set_property( GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake" )

set( URHO3D_HOME $ENV{URHO3D_HOME} CACHE PATH "Top directory containing Urho3D library and includes" )

list( APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/Modules" )
list( APPEND CMAKE_MODULE_PATH "${URHO3D_HOME}/share/Urho3D/CMake/Modules" )

#
# Project
#
project( Shmurho C CXX )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQURED ON )

option( SHMURHO_BUILD_DEMO "Build the demo game" ON )

#
# Global dependencies
#

include( UrhoCommon )
find_package( Urho3D REQUIRED )

#
# Modules
#
add_subdirectory( Shmurho/Phase )
add_subdirectory( Shmurho/Parcel )
if( SHMURHO_BUILD_DEMO )
	add_subdirectory( Shmurho/Demo )
endif()

install(
	DIRECTORY ./CMake
	DESTINATION share/Shmurho
)
install(
	FILES ./README.md ./LICENSE.md
	DESTINATION share/Shmurho
)

