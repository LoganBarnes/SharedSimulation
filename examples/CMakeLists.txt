cmake_minimum_required ( VERSION 3.6.0 )
project ( Examples )

function( BuildExample example )

  make_directory( ${CMAKE_BINARY_DIR}/${example}-build )

  execute_process(
                  COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" ${CMAKE_CURRENT_SOURCE_DIR}/${example}
                  RESULT_VARIABLE result
                  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${example}-build
                  )
  if( result )
    message( FATAL_ERROR "CMake step for ${example} failed: ${result}" )
  endif( )

  add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/${example} ${CMAKE_BINARY_DIR}/${example}-build )

endfunction( BuildExample )


BuildExample( 0_basic     )
BuildExample( 1_simple    )
BuildExample( 2_graphical )

