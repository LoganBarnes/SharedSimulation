

function( DownloadProject projectName )

#  # Download and unpack ${projectName} at configure time
#  configure_file( cmake/CMakeLists.txt.${projectName} ${CMAKE_CURRENT_BINARY_DIR}/${projectName}-download/CMakeLists.txt )
#  execute_process( COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
#    RESULT_VARIABLE result
#    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${projectName}-download )
#  if( result )
#    message( FATAL_ERROR "CMake step for ${projectName} failed: ${result}" )
#  endif( )
#  execute_process( COMMAND ${CMAKE_COMMAND} --build .
#    RESULT_VARIABLE result
#    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${projectName}-download )
#  if( result )
#    message( FATAL_ERROR "Build step for ${projectName} failed: ${result}" )
#  endif( )

endFunction( DownloadProject )
