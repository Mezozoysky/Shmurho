set( TARGET_NAME ShmurhoPreload )
define_source_files( GLOB_CPP_PATTERNS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
                     GLOB_H_PATTERNS "${CMAKE_CURRENT_SOURCE_DIR}/src/*hpp"
                     GROUP RECURSE )
set( INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src )
set( TARGET_PROPERTIES LINKER_LANGUAGE CXX )
setup_library( NODEPS STATIC )

target_include_directories( ${TARGET_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)

install(
	TARGETS ${TARGET_NAME}
	ARCHIVE DESTINATION lib
)
install(
	FILES ${H_FILES}
	DESTINATION include/Shmurho/Preload/
)

