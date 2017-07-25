# somber application

set( MODULE_NAME "Somber" )
set( MODULE_NSPATH "Shmurho/Somber" )
set( MODULE_TGT "somber" )

set( MODULE_ROOT ${CMAKE_CURRENT_SOURCE_DIR} )

set( MODULE_HEADERS "" )
set( MODULE_SOURCES "" )

# ===============================================
# ==== ${MODULE_ROOT}/${MODULE_NSPATH} group ====
# ===============================================
set( GROUP_HEADERS
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/App.hpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/GameState.hpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/MapLogic.hpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/ParcelLoader.hpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/PointsAndWaysData.hpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/DevKbdController.hpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/PhaseSwitcher.hpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/LoaderPhase.hpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/StartMenuPhase.hpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/WorldMapPhase.hpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/LocationPhase.hpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/CatacombPhase.hpp
)
set( GROUP_SOURCES
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/App.cpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/GameState.cpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/MapLogic.cpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/ParcelLoader.cpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/PointsAndWaysData.cpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/DevKbdController.cpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/PhaseSwitcher.cpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/LoaderPhase.cpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/StartMenuPhase.cpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/WorldMapPhase.cpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/LocationPhase.cpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/CatacombPhase.cpp
)
source_group( "${MODULE_NAME}\\${MODULE_NSPATH}"
  FILES ${GROUP_HEADERS} ${GROUP_SOURCES}
)
set( MODULE_HEADERS ${MODULE_HEADERS} ${GROUP_HEADERS} )
set( MODULE_SOURCES ${MODULE_SOURCES} ${GROUP_SOURCES} )

#
# Discover dependencies
#

#
# Build
#
set( TARGET_NAME ${MODULE_TGT} )
set( SOURCE_FILES ${MODULE_SOURCES} )
set( INCLUDE_DIRS ${MODULE_ROOT}/src ${MODULE_ROOT}/include ${URHO3D_INCLUDE_DIRS} )
set( LIBS ${MODULE_TGT_ARC} shmurho-parcel shmurho-phase )
set( TARGET_PROPERTIES LINKER_LANGUAGE CXX )
setup_main_executable()
