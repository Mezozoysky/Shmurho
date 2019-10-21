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
    message(FATAL_ERROR "${SCRIPT_NAME}:: Git remote isnt specified, use -DREMOTE")
endif()
if(NOT LOCAL)
    message(FATAL_ERROR "${SCRIPT_NAME}:: Git local directory isnt specified, use -DLOCAL")
endif()

include(ShmurhoGitUtils)

fetch_git_repo(REMOTE ${REMOTE}
    LOCAL ${LOCAL}
    GIT_CMD "${GIT_CMD}"
    BRANCH "${BRANCH}"
    DEPTH "${DEPTH}"
    SINGLE_BRANCH ${SINGLE_BRANCH}
    )

message("${SCRIPT_NAME} is done!")
