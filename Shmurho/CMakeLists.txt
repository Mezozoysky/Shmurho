option( SHMURHO_BUILD_PHASE "Build shmurho-phase library" ON )
option( SHMURHO_BUILD_LOADER "Build shmurho-loader library" ON )

if( SHMURHO_BUILD_PHASE )
    add_subdirectory( Phase )
endif()

if( SHMURHO_BUILD_LOADER )
    add_subdirectory( Loader )
endif()

