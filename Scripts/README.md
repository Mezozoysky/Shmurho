## Helper scpripts

- `fetch-urho3d.cmake` -- clone Urho3D repo (w/o history) onto local FS

    Usage: `cmake [options] -P editor-urho3d.cmake`.

    Possible options are:
    - `-DGIT_CMD=</path/to/git/executable>` (`git` by default)
    - `-DGIT_REMOTE=<remote_repo_url>` (`https://github.com/Mezozoysky/Urho3D.git` by default)
    - `-DGIT_LOCAL=</path/to/local/clone/dir>` (`${PROJECT_ROOT_DIR}/.build/Urho3D` by default)
    - `-DGIT_BRANCH=<branch_name>` (`master` by default)

- `build-urho3d.cmake` -- configure, build and install Urho3D

    Usage: `cmake [options] -P build-urho3d.cmake`

    Possible options are:
    - `-DCMAKE_CMD=</path/to/custom/cmake/executable>` (`cmake` by default)
    - `-DURHO3D_SOURCE_DIR=</path/to/Urho3D/project/clone>` (`${PROJECT_ROOT_DIR}` by default)
    - `-DURHO3D_BUILD_DIR=</path/toUrho3D/build/dir>` (`${URHO3D_SOURCE_DIR}-build` by default)
    - `-DURHO3D_INSTALL_DIR=</install/prefix/path>` (`${PROJECT_ROOT_DIR}/.build/SDK` by default)
