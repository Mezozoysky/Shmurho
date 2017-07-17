#
###: replace Prj/project.name
# Prj modules
#

###: for_each lib in project.modules.holibs replace LibHO/${lib}
#add_subdirectory( LibHO )
###: end_for_each

###: for_each lib in project.modules.libs replace Lib/${lib}
#add_subdirectory( Lib )
###: end_for_each

###: for_each app in project.modules.apps replace App/${app}.
add_subdirectory( App )
###: end_for_each

