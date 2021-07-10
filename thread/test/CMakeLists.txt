file( GLOB APP_SOURCES *.cc )
foreach( appfile ${APP_SOURCES} )
    # I used a simple string replace, to cut off .cc.
    string( REPLACE ".cc" "" app ${appfile} )
    # I used a simple string replace, to cut off pathinfo.
    string( REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" app ${app})
    message("${app} ${appfile}")
    add_executable( ${app} ${appfile} )
    # Make sure YourLib is linked to each app
    target_link_libraries( ${app} csapp )
endforeach( appfile ${APP_SOURCES} )