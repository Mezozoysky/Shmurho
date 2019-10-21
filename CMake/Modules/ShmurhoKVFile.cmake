cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

include(CMakeParseArguments)
include(ShmurhoCommon)

function(load_kv_file)
    # Usage:
    #       load_kv_file(
    #           PATH <path/to/kv/file>
    #           [PREFIX <key_prefix>]
    #       )
    cmake_parse_arguments(ARGS # prefix for parsed args
                          "" # list of boolean args
                          "PATH;PREFIX" # list of single value args
                          "" # list of multi value args
                          ${ARGN} # parse all arguments
                          )
    if(NOT ARGS_PATH)
        message(FATAL_ERROR "read_key_val_file():: File path isnt specified, use PATH")
    endif()
    set(prefix "KV_")
    if(ARGS_PREFIX)
        set(prefix ${ARGS_PREFIX})
    endif()

    file(STRINGS ${ARGS_PATH} file_contents)
    foreach(kv_line ${file_contents})
        # message("------ kv line: \"${kv_line}\"")
        string(REGEX REPLACE "^[ \t]+" "" kv_line "${kv_line}")
        # message("------ stripped kv line: \"${kv_line}\"")
        string(REGEX MATCH "^#+.*" kv_comment "${kv_line}")
        if(kv_comment)
            # message("------ comment: \"${kv_comment}\"")
            continue()
        endif()
        string(REGEX MATCH "^[^=]+" kv_key "${kv_line}")
        # message("------ kv key: \"${kv_key}\"")
        string(REGEX REPLACE "^${kv_key}=" "" kv_val "${kv_line}")
        # message("------ kv val: \"${kv_val}\"")
        set(var_name "${prefix}${kv_key}")
        # message("------ kv var name: \"${var_name}\"")
        set("${var_name}" "${kv_val}" PARENT_SCOPE)
    endforeach()
endfunction()


function(save_kv_file)
    # Usage:
    #       save_kv_file(
    #           PATH <path/to/kv/file> # kv file path
    #           PREFIX <key_prefix> # save all variable wich start with the key_prefix
    #           [DROP_PREFIX] # save keys without prefixes
    #       )
    cmake_parse_arguments(ARGS # prefix for parsed args
                          "DROP_PREFIX" # list of boolean args
                          "PATH;PREFIX" # list of single value args
                          "" # list of multi value args
                          ${ARGN} # parse all arguments
                          )
    if(NOT ARGS_PATH)
        message(FATAL_ERROR "save_kv_file():: File path isnt specified, use PATH")
    endif()
    if(NOT ARGS_PREFIX)
        message(FATAL_ERROR "save_kv_file():: Prefix isnt specified, use PREFIX")
    endif()

    collect_variables(PREFIX "${ARGS_PREFIX}" OUTPUT_VAR kv_keys ECHO)
    set(kv_lines)
    set(prev_key)
    foreach(kv_key ${kv_keys})
        set(stripped_key "${kv_key}")
        if(ARGS_DROP_PREFIX)
            string(REGEX REPLACE "^${ARGS_PREFIX}" "" stripped_key "${kv_key}")
        endif()
        set(safe_val "${${kv_key}}") # if var is list then join it with ';'
        string(JOIN " " safe_val ${safe_val})
        set(kv_lines ${kv_lines} "${stripped_key}=${safe_val}")
    endforeach()
    set(kv_keys) # kv_keys are not needed since this moment
    string(JOIN "\n" kv_lines ${kv_lines})
    file(WRITE "${ARGS_PATH}" "${kv_lines}")
endfunction()
