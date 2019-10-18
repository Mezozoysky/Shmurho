cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

if(NOT CMAKE_SCRIPT_MODE_FILE OR CMAKE_PARENT_LIST_FILE)
    message(FATAL_ERROR "This file '${CMAKE_CURRENT_LIST_FILE}' should be used with 'cmake -P'")
endif()

get_filename_component(SCRIPT_NAME "${CMAKE_SCRIPT_MODE_FILE}" NAME)
get_filename_component(SCRIPTS_DIR "${CMAKE_SCRIPT_MODE_FILE}" DIRECTORY)
get_filename_component(PROJECT_ROOT_DIR "${SCRIPTS_DIR}" DIRECTORY)


message("${SCRIPT_NAME} starts!")
set(LOG_DIR "${SCRIPTS_DIR}/log")

if(NOT CMAKE_CMD)
    set(CMAKE_CMD "cmake")
endif()
if(NOT URHO3D_SOURCE_DIR)
    set(URHO3D_SOURCE_DIR "${PROJECT_ROOT_DIR}/.build/Urho3D")
endif()
if(NOT URHO3D_BUILD_DIR)
    set(URHO3D_BUILD_DIR "${URHO3D_SOURCE_DIR}-build")
endif()
if(NOT URHO3D_INSTALL_DIR)
    set(URHO3D_INSTALL_DIR "${PROJECT_ROOT_DIR}/.build/SDK")
endif()

message("Source dir: ${URHO3D_SOURCE_DIR}")
message("Build dir: ${URHO3D_BUILD_DIR}")
message("Install dir: ${URHO3D_INSTALL_DIR}")

set(CONFIG_ARGS "../Urho3D" "-DCMAKE_INSTALL_PFEFIX=\"${URHO3D_INSTALL_DIR}\"")
set(CONFIG_EXIT_CODE)
set(CONFIG_LOG_FILE "${LOG_DIR}/${SCRIPT_NAME}-configure.log")
message("Configuring with args: ${CONFIG_ARGS}")
execute_process(COMMAND ${CMAKE_CMD} ${CONFIG_ARGS}
    RESULT_VARIABLE CONFIG_EXIT_CODE
    OUTPUT_VARIABLE CONFIG_OUTPUT
    ERROR_VARIABLE CONFIG_OUTPUT
    WORKING_DIRECTORY "${URHO3D_BUILD_DIR}")
file(WRITE "${CONFIG_LOG_FILE}" ${CONFIG_OUTPUT})
if(NOT (CONFIG_EXIT_CODE EQUAL 0))
    message(FATAL_ERROR "Error while configuring Urho3D project: ${CONFIG_EXIT_CODE}; See configure log at \"${CONFIG_LOG_FILE}\"")
endif()
message("Configuring done! See full configure log at \"${CONFIG_LOG_FILE}\".")

set(BUILD_ARGS --build .)
set(BUILD_EXIT_CODE)
set(BUILD_LOG_FILE "${LOG_DIR}/${SCRIPT_NAME}-build.log")
message("Building with args: ${BUILD_ARGS}")
execute_process(COMMAND ${CMAKE_CMD} ${BUILD_ARGS}
    RESULT_VARIABLE BUILD_EXIT_CODE
    OUTPUT_VARIABLE BUILD_OUTPUT
    ERROR_VARIABLE BUILD_OUTPUT
    WORKING_DIRECTORY "${URHO3D_BUILD_DIR}")
file(WRITE "${BUILD_LOG_FILE}" ${BUILD_OUTPUT})
if(NOT (BUILD_EXIT_CODE EQUAL 0))
    message(FATAL_ERROR "Error while building Urho3D project: ${BUILD_EXIT_CODE}; See build log at \"${BUILD_LOG_FILE}\"")
endif()
message("Building done! See full build log at \"${BUILD_LOG_FILE}\".")

set(INSTALL_ARGS --build . --target install)
set(INSTALL_EXIT_CODE)
set(INSTALL_LOG_FILE "${LOG_DIR}/${SCRIPT_NAME}-install.log")
message("Installing with args: ${INSTALL_ARGS}")
execute_process(COMMAND ${CMAKE_CMD} ${INSTALL_ARGS}
    RESULT_VARIABLE INSTALL_EXIT_CODE
    OUTPUT_VARIABLE INSTALL_OUTPUT
    ERROR_VARIABLE INSTALL_OUTPUT
    WORKING_DIRECTORY "${URHO3D_BUILD_DIR}")
file(WRITE "${INSTALL_LOG_FILE}" ${INSTALL_OUTPUT})
if(NOT (INSTALL_EXIT_CODE EQUAL 0))
    message(FATAL_ERROR "Error while installing Urho3D project: ${INSTALL_EXIT_CODE}; See install log at \"${INSTALL_LOG_FILE}\"")
endif()
message("Installation done! See full install log at \"${INSTALL_LOG_FILE}\".")

message("${SCRIPT_NAME} is done!")
