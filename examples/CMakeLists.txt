cmake_minimum_required ( VERSION 3.6.0 )
project ( Examples )

include( ExternalProject )

function( BuildExample example )

  ExternalProject_Add( ${example}-external
    DOWNLOAD_COMMAND   ""
    SOURCE_DIR         ${CMAKE_CURRENT_SOURCE_DIR}/${example}
    BINARY_DIR         ${PROJECT_BINARY_DIR}/${example}-build
    BUILD_ALWAYS       1
    INSTALL_DIR        ${PROJECT_BINARY_DIR}
    CMAKE_ARGS         -DCMAKE_INSTALL_PREFIX:STRING=<INSTALL_DIR>
                       -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
  )

endfunction( BuildExample )

BuildExample( 0_basic     )
BuildExample( 1_simple    )
BuildExample( 2_graphical )

