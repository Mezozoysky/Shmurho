cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

if(NOT CMAKE_SCRIPT_MODE_FILE OR CMAKE_PARENT_LIST_FILE)
    message(FATAL_ERROR "This file '${CMAKE_CURRENT_LIST_FILE}' should be used with 'cmake -P'")
endif()

get_filename_component(SCRIPT_NAME "${CMAKE_SCRIPT_MODE_FILE}" NAME)
get_filename_component(PRJ_SCRIPTS_DIR "${CMAKE_SCRIPT_MODE_FILE}" DIRECTORY)
get_filename_component(PRJ_CMAKE_DIR "${PRJ_SCRIPTS_DIR}" DIRECTORY)
get_filename_component(PRJ_ROOT_DIR "${PRJ_CMAKE_DIR}" DIRECTORY)
set(PRJ_MODULES_DIR "${PRJ_ROOT_DIR}/CMake/Modules")
set(PRJ_SOURCE_DIR "${PRJ_ROOT_DIR}")
set(PRJ_BUILD_DIR "${PRJ_ROOT_DIR}/build")
set(PRJ_DEPS_DIR "${PRJ_ROOT_DIR}/deps")

list(INSERT CMAKE_MODULE_PATH 0 "${PRJ_MODULES_DIR}")


message("${SCRIPT_NAME} starts!")

if(NOT REMOTE)
    set(REMOTE "https://github.com/Mezozoysky/Urho3D.git")
endif()
if(NOT LOCAL)
    set(LOCAL "${PRJ_ROOT_DIR}/Urho3D")
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
