cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

if(NOT CMAKE_SCRIPT_MODE_FILE OR CMAKE_PARENT_LIST_FILE)
    message(FATAL_ERROR "This file '${CMAKE_CURRENT_LIST_FILE}' should be used with 'cmake -P'")
endif()

get_filename_component(SCRIPT_NAME "${CMAKE_SCRIPT_MODE_FILE}" NAME)
get_filename_component(SCRIPTS_DIR "${CMAKE_SCRIPT_MODE_FILE}" DIRECTORY)
get_filename_component(PROJECT_ROOT_DIR "${SCRIPTS_DIR}" DIRECTORY)


message("${SCRIPT_NAME} starts!")

if(NOT GIT_CMD)
    set(GIT_CMD "git")
endif()
if(NOT GIT_BRANCH)
    set(GIT_BRANCH "master")
endif()
if(NOT GIT_REMOTE)
    set(GIT_REMOTE "https://github.com/Mezozoysky/Urho3D.git")
endif()
if(NOT GIT_LOCAL)
    set(GIT_LOCAL "${PROJECT_ROOT_DIR}/.build/Urho3D")
endif()

set(GIT_ARGS clone
    --depth 1
    --single-branch
    --branch ${GIT_BRANCH}
    --
    "${GIT_REMOTE}"
    "${GIT_LOCAL}")

set(GIT_EXIT_CODE)
execute_process(COMMAND ${GIT_CMD} ${GIT_ARGS}
    RESULT_VARIABLE GIT_EXIT_CODE)
if(NOT (GIT_EXIT_CODE EQUAL 0))
    message(FATAL_ERROR "git clone execution error: ${GIT_EXIT_CODE}")
endif()

message("${SCRIPT_NAME} is done!")
