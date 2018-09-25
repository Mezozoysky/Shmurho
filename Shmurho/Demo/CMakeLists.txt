# demo application

set( MODULE_NAME "Demo" )
set( MODULE_NSPATH "Shmurho/Demo" )
set( MODULE_TGT "shmurho-demo" )

set( MODULE_ROOT ${CMAKE_CURRENT_SOURCE_DIR} )

set( MODULE_HEADERS "" )
set( MODULE_SOURCES "" )


set( TARGET_NAME ${MODULE_TGT} )
# set( SOURCE_FILES ${MODULE_SOURCES} )
define_source_files( GLOB_CPP_PATTERNS "${MODULE_ROOT}/src/*.cpp"
                     GLOB_H_PATTERNS "${MODULE_ROOT}/src/*hpp"
                     RECURSE GROUP )
set( INCLUDE_DIRS ${MODULE_ROOT}/src ${MODULE_ROOT}/include )
set( LIBS shmurho-loader shmurho-phase )
set( TARGET_PROPERTIES LINKER_LANGUAGE CXX )
setup_main_executable()


add_custom_command(
    TARGET ${MODULE_TGT}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${MODULE_ROOT}/assets/ ${CMAKE_BINARY_DIR}/share/Shmurho/Demo/
)


install(
    TARGETS ${MODULE_TGT}
	#BUNDLE DESTINATION bin
	RUNTIME DESTINATION bin
)
install(
	DIRECTORY ${MODULE_ROOT}/assets/Data ${MODULE_ROOT}/assets/CoreData
	DESTINATION share/Shmurho/Demo
)
