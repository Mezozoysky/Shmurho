# suctom target docs

if ( SHMURHO_BUILD_DOCS )
    if ( NOT DOXYGEN_FOUND )
         message( FATAL_ERROR "Doxygen is needed to build the documentation." )
    endif()

    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Shmurho.doxygen)
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/doxyfile)

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    message( "Doxygen build started." )

    add_custom_target( docs
                      COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile_in}
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                      COMMENT "Generating API documentation with Doxygen"
                      VERBATIM )

                  install( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/html DESTINATION share/doc/Shmurho )
endif()
