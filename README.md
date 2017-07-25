# Shmurho - helper libraries for Urho3D engine

## Status ##
Just started

## License ##
*Shmurho* is released under the terms of zlib/png license;
see full license text in LICENSE.md file or at https://opensource.org/licenses/Zlib

## Get/Build/Install ##
	git clone git@github.com:Mezozoysky/Shmurho.git
	cd Shmurho
	mkdir build
	cd build
	cmake ../Shmurho [-DURHO3D_HOME="<urho3d_home>"] [-DCMAKE_INSTALL_PREFIX="<install_prefix>"]
	make
	make install # or 'sudo make install' if <install_prefix> isn't in user's paths

`urho3d_home` - Path to Urho3D installation (default - contents of `URHO3D_HOME` env variable or `/usr/local`).

`install_prefix` - Path where to install Shmurho (default - `/usr/local`).

## Use ##
Include in your `CMakeLists.txt`:

	set( ENV{SHMURHO_HOME} <install_prefix> )
	find_package( Shmurho REQUIRED Phase Parcel )

And some variables will be defined for You:

* `Shmurho_INCLUDE_DIR` - path to Shmurho includes

* `Shmurho_LIBRARIES` - list of all found Shmurho libraries

* `Shmurho_XXX_LIBRARY` - path to found library (XXX is for Phase, Parcel, etc)

* `Shmurho_FOUND` - TRUE if libraries are found

## Documentation ##
Not yet established

