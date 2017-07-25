# urho-based library

set( MODULE_NAME "Phase" )
set( MODULE_NSPATH "Shmurho/Phase" )
set( MODULE_TGT "shmurho-phase" )

set( MODULE_ROOT ${CMAKE_CURRENT_SOURCE_DIR} )

set( MODULE_HEADERS "" )
set( MODULE_SOURCES "" )

# ===============================================
# ==== ${MODULE_ROOT}/${MODULE_NSPATH} group ====
# ===============================================
set( GROUP_HEADERS
    ${MODULE_ROOT}/include/${MODULE_NSPATH}/Switcher.hpp
    ${MODULE_ROOT}/include/${MODULE_NSPATH}/PhaseEvents.hpp
    ${MODULE_ROOT}/include/${MODULE_NSPATH}/Partaker.hpp
)
set( GROUP_SOURCES
    ${MODULE_ROOT}/src/${MODULE_NSPATH}/Switcher.cpp
)
source_group( "${MODULE_NAME}\\${MODULE_NSPATH}"
  FILES ${GROUP_HEADERS} ${GROUP_SOURCES}
)
set( MODULE_HEADERS ${MODULE_HEADERS} ${GROUP_HEADERS} )
set( MODULE_SOURCES ${MODULE_SOURCES} ${GROUP_SOURCES} )


set( TARGET_NAME ${MODULE_TGT} )
set( SOURCE_FILES ${MODULE_SOURCES} )
set( INCLUDE_DIRS ${MODULE_ROOT}/src ${MODULE_ROOT}/include ${URHO3D_INCLUDE_DIRS} )
set( TARGET_PROPERTIES LINKER_LANGUAGE CXX )
setup_library( NODEPS STATIC )

target_include_directories( ${MODULE_TGT}
	PUBLIC
    $<BUILD_INTERFACE:${MODULE_ROOT}/include>
    $<INSTALL_INTERFACE:include>
)

install(
	TARGETS ${MODULE_TGT}
	ARCHIVE DESTINATION lib
)
install(
	FILES ${MODULE_HEADERS}
	DESTINATION include/${MODULE_NSPATH}
)

