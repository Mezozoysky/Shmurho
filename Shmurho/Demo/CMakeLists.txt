# demo application

set( MODULE_NAME "Demo" )
set( MODULE_NSPATH "Shmurho/Demo" )
set( MODULE_TGT "shmurho-demo" )

set( MODULE_ROOT ${CMAKE_CURRENT_SOURCE_DIR} )

set( MODULE_HEADERS "" )
set( MODULE_SOURCES "" )

# ===============================================
# ==== ${MODULE_ROOT}/${MODULE_NSPATH} group ====
# ===============================================
set( GROUP_HEADERS
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/App.hpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/GameState.hpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/ParcelLoader.hpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/DevKbdController.hpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/PhaseSwitcher.hpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/Loader.hpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/LoaderEvents.hpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/StartMenu.hpp
)
set( GROUP_SOURCES
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/App.cpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/GameState.cpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/ParcelLoader.cpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/DevKbdController.cpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/PhaseSwitcher.cpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/Loader.cpp
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/StartMenu.cpp
    ${MODULE_ROOT}/src/main.cpp
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
set( LIBS shmurho-parcel shmurho-phase )
set( TARGET_PROPERTIES LINKER_LANGUAGE CXX )
setup_main_executable()

#
# Post-build
#
add_custom_command(
    TARGET ${MODULE_TGT}
    POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory ${MODULE_ROOT} $<TARGET_FILE_DIR:"${MODULE_TGT}">
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${MODULE_ROOT}/assets/ ${CMAKE_BINARY_DIR}/share/Shmurho/Demo/
)

#
# Install
#

install(
    TARGETS ${MODULE_TGT}
	#BUNDLE DESTINATION bin
	RUNTIME DESTINATION bin
)
install(
	DIRECTORY ${MODULE_ROOT}/assets/Data ${MODULE_ROOT}/assets/CoreData
	DESTINATION share/Shmurho/Demo
)
