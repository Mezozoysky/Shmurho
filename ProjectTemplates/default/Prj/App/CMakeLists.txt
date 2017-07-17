# Application module

set( MODULE_NAME "App" )
set( MODULE_NSPATH "App" )
set( MODULE_TGT "app" )

set( MODULE_ROOT ${CMAKE_CURRENT_SOURCE_DIR} )

set( MODULE_HEADERS "" )
set( MODULE_SOURCES "" )

# ===============================================
# ==== ${MODULE_ROOT}/${MODULE_NSPATH} group ====
# ===============================================
set( GROUP_HEADERS
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/Application.hpp
	${MODULE_ROOT}/src/${MODULE_NSPATH}/PhaseSwitcher.hpp
	${MODULE_ROOT}/src/${MODULE_NSPATH}/ParcelLoader.hpp
)
set( GROUP_SOURCES
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/Application.cpp
	${MODULE_ROOT}/src/${MODULE_NSPATH}/PhaseSwitcher.cpp
	${MODULE_ROOT}/src/${MODULE_NSPATH}/ParcelLoader.cpp
)
source_group( "${MODULE_NAME}\\${MODULE_NSPATH}"
	FILES ${GROUP_HEADERS} ${GROUP_SOURCES}
)
set( MODULE_HEADERS ${MODULE_HEADERS} ${GROUP_HEADERS} )
set( MODULE_SOURCES ${MODULE_SOURCES} ${GROUP_SOURCES} )


include( UrhoCommon )
find_package( Urho3D REQUIRED )
find_package( Shmurho REQUIRED Phase Parcel )

set( TARGET_NAME ${MODULE_TGT} )
set( SOURCE_FILES ${MODULE_SOURCES} )
set( INCLUDE_DIRS ${MODULE_ROOT}/include ${Shmurho_INCLUDE_DIR} ${URHO3D_INCLUDE_DIRS} )
set( LIBS ${Shmurho_LIBRARIES} )
set( TARGET_PROPERTIES LINKER_LANGUAGE CXX )
setup_main_executable()

file( COPY ${MODULE_ROOT}/share DESTINATION ${CMAKE_BINARY_DIR} )

