cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

include(CMakeParseArguments)
function(fetch_git_repo)
    # Usage:
    #       fetch_git_repo(
    #           [GIT_CMD <path/to/git/executable>]
    #           [SINGLE_BRANCH]
    #           [BRANCH <branch>]
    #           [DEPTH <depth>]
    #           REMOTE <remote/url>
    #           LOCAL <local/dir/path>
    #       )
    cmake_parse_arguments(ARGS # prefix for parsed args
        "SINGLE_BRANCH" # list of boolean args
        "GIT_CMD;REMOTE;LOCAL;BRANCH;DEPTH" # list of single value args
        "" # list of multi value args
        ${ARGN}) # parse all arguments

    set(git_cmd "git")
    if(ARGS_GIT_CMD)
        set(git_cmd "${ARGS_GIT_CMD}")
    endif()
    set(branch)
    if(ARGS_BRANCH)
        set(branch ${ARGS_BRANCH})
    endif()
    set(single_branch)
    if(ARGS_SINGLE_BRANCH)
        set(single_branch "--single-branch")
    endif()
    set(depth 0)
    if(ARGS_DEPTH)
        if(ARGS_DEPTH MATCHES "^[0-9]+$")
            set(depth ${ARGS_DEPTH})
        else()
            message(WARNING
                "fetch_git_repo():: Clone depth should be a valid integer, but '${ARGS_DEPTH}' given. Default value (0) will used.")
        endif()
    endif()
    if(NOT ARGS_REMOTE)
        message(FATAL_ERROR "fetch_git_repo():: Git remote isnt specified, use REMOTE")
    endif()
    set(remote ${ARGS_REMOTE})
    if(NOT ARGS_LOCAL)
        message(FATAL_ERROR "fetch_git_repo():: Git local directory isnt specified, use LOCAL")
    endif()
    set(local ${ARGS_LOCAL})

    set(git_args clone)
    if(NOT (depth MATCHES "^0$"))
        set(git_args ${git_args} --depth ${depth})
    endif()

    set(git_args ${git_args} ${single_branch})

    if(branch)
        set(git_args ${git_args} --branch ${branch})
    endif()

    set(git_args ${git_args} --
        "${remote}"
        "${local}")

    message(STATUS "fetch_git_repo():: Git args: " ${git_args})

    set(git_exit_code)
    execute_process(COMMAND ${git_cmd} ${git_args}
        RESULT_VARIABLE git_exit_code)
    if(NOT (git_exit_code EQUAL 0))
        message(FATAL_ERROR "fetch_git_repo():: 'git clone' execution error: ${git_exit_code}")
    endif()
endfunction(fetch_git_repo)

