## Getting started

Assuming:
1. Urho3D engine is already installed onto local host.
2. Shmurho project repo is already cloned onto local filesystem.

### Build Shmurho

CMake is used as the build system:

    cd Shmurho
    mkdir build
    cd build
    cmake .. [BUILD_OPTIONS]         # configure
    cmake --build .                  # build
    cmake --build . --target docs    # optional build docs
    cmake --build . --target install # install

`BUILD_OPTIONS`: 
* `-DURHO3D_HOME=<PATH>`

 Use PATH as Urho3D home directory. By default environment variable URHO3D_HOME
or '/usr/local' will be used.

* `-DCMAKE_INSTALL_PREFIX=<PATH>`

 Use PATH as Shmurho install prefix.


### Set up basic Urho3D+Shmurho project

Assuming basic project layout:

    MyProject/
    + Sources/       # source files will be here
    + Resources/
      + CoreData/    # core Urho3D resource files will be here
      + Data/        # MyProjects resource files will be here


#### Set up default Urho3D application

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
    if( URHO3D_HOME )
        list( APPEND CMAKE_MODULE_PATH "${URHO3D_HOME}/share/Urho3D/CMake/Modules/" )
        set( URHO3D_RESOURCES_DIR "${URHO3D_HOME}/share/Resources/")
    endif()
    
    project( Arcade C CXX )
    
    set( CMAKE_CXX_STANDARD 11 )
    set( CMAKE_CXX_STANDARD_REQURED ON )
    
    include( UrhoCommon )
    find_package( Urho3D REQUIRED )
    
    add_subdirectory( Sources/ )

Add `MyProject/Sources/CMakeLists.txt` file with the following contents:

    set( TARGET_NAME MyProject )
    
    define_source_files( GLOB_CPP_PATTERNS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
                         GLOB_H_PATTERNS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
                         RECURSE GROUP)
    define_resource_dirs( GLOB_PATTERNS
                          ${CMAKE_SOURCE_DIR}/Resources/Data
                          ${CMAKE_SOURCE_DIR}/Resources/CoreData )
    
    set( INCLUDE_DIRS
         ${CMAKE_CURRENT_SOURCE_DIE}
         ${URHO3D_INCLUDE_DIRS} )
    
    set( TARGET_PROPERTIES LINKER_LANGUAGE CXX )
    setup_main_executable()

Add `MyProject/Sources/main.cpp` file with the following contents:

    #include <Urho3D/Engine/Application.h>
    
    URHO3D_DEFINE_APPLICATION_MAIN(Urho3D::Application);

Copy contents of `${URHO3D_HOME}/share/Urho3D/Resources/CoreData/` folder into `MyProject/Resources/CoreData/`
folder (or create symlink).

Create build directory `MyProject/build` and `cd` to it.

Run the following commands inside build directory to configure build system with possible options and
build project like this:

    cmake .. -DURHO3D_HOME=<PATH/TO/URHO3D> # configure
    cmake --build . # build

See results in `MyProject/build/bin/` folder.

> At this point we have empty default application -- it shows fullscreen black rectangle and quits on
> Alt+F4 / Cmd+Q / etc., that is all.


#### Add MyApplication class

Default Urho3D application designed for inheritance and mostly does nothing by itself so we should derive from
Urho3D::Application.

Add `MyProject/Sources/MyApplication.hpp` file with the following contents:

    #pragma once
    
    #include <Urho3D/Engine/Application.h>
    
    namespace Urho3D {
    class Context;
    };
    
    namespace My
    {
    
    class MyApplication
    : public Urho3D::Application
    {
        URHO3D_OBJECT(MyApplication, Urho3D::Application);
    
    public:
        explicit MyApplication(Urho3D::Context* context);
        virtual ~MyApplication() = default;
    
        virtual void Setup() override;
        virtual void Start() override;
        virtual void Stop() override;
    };
    
    } // namespace My

Add `MyProject/Sources/MyApplication.cpp` file the following contents:

    #include "MyApplication.hpp"
    
    using Urho3D::Context;
    
    namespace My
    {
    
    MyApplication::MyApplication(Context* context)
    : Urho3D::Application(context)
    {
    }
    
    void MyApplication::Setup()
    {
    }
    
    void MyApplication::Start()
    {
    }
    
    void MyApplication::Stop()
    {
    }
    
    } // namespace My

Build project:

    cd MyProject/build
    cmake --build .

> At this point our application is still empty but now we can extend it as needed.

#### Add Shmurho libraries into project setup

Open `MyProject/CMakeLists.txt` with editor and make some addition:

We need a way to specify a path to Shmurho, so create 'cached' CMake variable `SHMURHO_HOME` (and load it from
`SHMURHO_HOME` environment variable by default), append `CMAKE_MODULE_PATH` with path to Shmurho CMake modules
(related to `SHMURHO_HOME`):

    set( SHMURHO_HOME $ENV{SHMURHO_HOME} CACHE PATH "Top directory containing Shmurho library and includes" )
    if( SHMURHO_HOME )
        list( APPEND CMAKE_MODULE_PATH "${SHMURHO_HOME}/share/Shmurho/CMake/Modules/" )
    endif()

Next, use find_package() to find Shmurho libraries and headers:

    find_package( Shmurho REQUIRED Phase Preload )

Save `MyProject/CMakeLists.txt` file. Now it should look like this:

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
    if( URHO3D_HOME )
        list( APPEND CMAKE_MODULE_PATH "${URHO3D_HOME}/share/Urho3D/CMake/Modules/" )
        set( URHO3D_RESOURCES_DIR "${URHO3D_HOME}/share/Resources/")
    endif()
    
    set( SHMURHO_HOME $ENV{SHMURHO_HOME} CACHE PATH "Top directory containing Shmurho library and includes" )
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


Open `MyProject/Sources/CMakeLists.txt` file with editor and:

Add `${Shmurho_INCLUDE_DIR}` into `INCLUDE_DIRS` list.

Add the following line to link our application with Shmurho libraries:

    set( LIBS ${Shmurho_Phase_LIBRARY} ${Shmurho_Preload_LIBRARY} )

Save `MyProject/Sources/CmakeLists.txt` file, now it should look like this:

    set( TARGET_NAME MyProject )
    
    define_source_files( GLOB_CPP_PATTERNS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
                         GLOB_H_PATTERNS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
                         RECURSE GROUP )
    define_resource_dirs( GLOB_PATTERNS
                          ${CMAKE_SOURCE_DIR}/Resources/Data
                          ${CMAKE_SOURCE_DIR}/Resources/CoreData )
    
    set( INCLUDE_DIRS
         ${CMAKE_CURRENT_SOURCE_DIR}
         ${Shmurho_INCLUDE_DIR}
         ${URHO3D_INCLUDE_DIRS} )
    
    set( LIBS ${Shmurho_Phase_LIBRARY} ${Shmurho_Preload_LIBRARY} )
    set( TARGET_PROPERTIES LINKER_LANGUAGE CXX )
    
    setup_main_executable()

Reconfigure build system:

    cd MyProject/build
    cmake .. -DURHO3D_HOME=<PATH/TO/URHO3D> -DSHMURHO_HOME=<PATH/TO/SHMURHO>

Rebuild project:

    cd MyProject/build
    cmake --build . --target clean
    cmake --build .

> At this point we have empty but extendable Shmurho-ready application. This can be used as template for empty
> Urho3D+Shmurho application project.
