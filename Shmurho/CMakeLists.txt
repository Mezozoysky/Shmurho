option( SHMURHO_BUILD_PHASE "Build ShmurhoPhase library" ON )
option( SHMURHO_BUILD_PRELOAD "Build ShmurhoPreload library" ON )

if( SHMURHO_BUILD_PHASE )
    add_subdirectory( Phase )
endif()

if( SHMURHO_BUILD_PRELOAD )
    add_subdirectory( Preload )
endif()

