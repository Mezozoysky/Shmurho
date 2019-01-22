## Getting started

Assuming:
1. Urho3D engine is already installed onto local host.
2. Shmurho project repo is already cloned onto local filesystem.

### Build Shmurho

CMake is used as the build system:

    cd Shmurho
    mkdir build
    cd build
    cmake .. [BUILD_OPTIONS] # configure
    cmake --build .
    cmake --build . --target docs # optional
    cmake --build . --target install

`BUILD_OPTIONS`: 
* `-DURHO3D_HOME=<PATH>`

 Use PATH as Urho3D home directory. By default environment variable URHO3D_HOME
or '/usr/local' will be used.

* `-DCMAKE_INSTALL_PREFIX=<PATH>`

 Use PATH as Shmurho install prefix.

### Set up basic Urho3D/Shmurho based game/application project

Assuming basic project layout:

    MyProject/
    + Sources/       # source files will be here
    + Resources/
      + CoreData/    # core Urho3D resource files will be here
      + Data/        # MyProjects resource files will be here

Add `MyProject/CMakeLists.txt` file with the following contents:

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
    
    set( URHO3D_HOME $ENV{URHO3D_HOME} CACHE PATH "Top directory containing Urho3D library and includes" )
    set( SHMURHO_HOME $ENV{SHMURHO_HOME} CACHE PATH "Top directory containing Shmurho library and includes" )
    
    if( URHO3D_HOME )
        list( APPEND CMAKE_MODULE_PATH "${URHO3D_HOME}/share/Urho3D/CMake/Modules/" )
        set( URHO3D_RESOURCES_DIR "${URHO3D_HOME}/share/Resources/")
    endif()
    if( SHMURHO_HOME )
        list( APPEND CMAKE_MODULE_PATH "${SHMURHO_HOME}/share/Shmurho/CMake/Modules/" )
    endif()
    
    project( Arcade C CXX )
    
    set( CMAKE_CXX_STANDARD 11 )
    set( CMAKE_CXX_STANDARD_REQURED ON )
    
    include( UrhoCommon )
    find_package( Urho3D REQUIRED )
    find_package( Shmurho REQUIRED Phase Preload )
    
    add_subdirectory( Sources/ )

Add `MyProject/Sources/CMakeLists.txt` file with the following contents:

    set( TARGET_NAME MyProject )
    define_source_files( GLOB_CPP_PATTERNS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
                         GLOB_H_PATTERNS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
                         RECURSE GROUP)
    define_resource_dirs( GLOB_PATTERNS ${CMAKE_SOURCE_DIR}/Resources/Data ${CMAKE_SOURCE_DIR}/Resources/CoreData )
    
    set( INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIE}
        ${Shmurho_INCLUDE_DIR}
        ${URHO3D_INCLUDE_DIRS} )
    
    set( LIBS ${Shmurho_Phase_LIBRARY} ${Shmurho_Preload_LIBRARY} )
    set( TARGET_PROPERTIES LINKER_LANGUAGE CXX )
    setup_main_executable()

Add `MyProject/Sources/main.cpp` file with the following contents:

    #include <Urho3D/Engine/Application.h>
    
    URHO3D_DEFINE_APPLICATION_MAIN(Urho3D::Application);

At this point we have a very empty application -- it shows fullscreen black rectangle and quits on Alt+F4 /
Cmd+Q / etc., that is all.
