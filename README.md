## Shmurho ##
Shmurho is a set of helper libraries for C++ development with Urho3D engine.

Goal is to provide some features, which are not provided (and/or should
not be provided) by Urho3D itself but are periodically useful in (my)
Urho3D-based gamedev practice.

__Libraries__:

* __Phase__

 ShmurhoPhase library helps to orginize game/app with distinct phases (like
main menu phase, splash screen phase, game level phase, etc).

* __Preload__

 ShmurhoPreload library provides ability to background load resources lists
(parcels) and one-point interface for background loading resources and scenes.
Prefabs support and more background operations are planned.

### Status ###
Project is NOT production ready, but anyone can improve it.

### License ###
Shmurho is released under the terms of zlib/png license; see full license text
in LICENSE.md file or at https://opensource.org/licenses/Zlib

### Get/Build/Install ###
    git clone git@github.com:Mezozoysky/Shmurho.git
    cd Shmurho
    mkdir build
    cd build
    cmake .. [-DURHO3D_HOME="<urho3d_home>"] [-DCMAKE_INSTALL_PREFIX="<install_prefix>"]
    cmake --build .
    cmake --build . --target install # use sudo if <install_prefix> isn't in user's paths

`urho3d_home` - Path to Urho3D installation (default - contents of `URHO3D_HOME`
env variable or `/usr/local`).

`install_prefix` - Path where to install Shmurho (default - `/usr/local`).

### Use ###
Include in your `CMakeLists.txt`:

    set( SHMURHO_HOME <install_prefix> )
    list( APPEND CMAKE_MODULE_PATH "${SHMURHO_HOME}/share/Shmurho/CMake/Modules" )
    find_package( Shmurho REQUIRED Phase Parcel )

And some variables will be defined for You:

* `Shmurho_INCLUDE_DIR` - path to Shmurho includes

* `Shmurho_LIBRARIES` - list of all found Shmurho libraries

* `Shmurho_XXX_LIBRARY` - every single library found (XXX is for Phase, Preload, etc)

* `Shmurho_FOUND` - TRUE if libraries are found

### Documentation ###
Not yet established

