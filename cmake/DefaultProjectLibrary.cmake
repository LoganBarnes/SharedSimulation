
if ( PROJECT_CONFIG_FILE )

  configure_file (
                  ${PROJECT_CONFIG_FILE}
                  ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.hpp
                  )

endif( ) # PROJECT_CONFIG_FILE



set ( PROJECT_INCLUDE_DIRS ${PROJECT_INCLUDE_DIRS} ${PROJECT_BINARY_DIR} )

# Create named folders for the sources within the .vcproj
# Empty name lists them directly under the .vcproj
source_group( "" FILES ${PROJECT_SOURCE}      )


# create a cude lib if necessary
if ( PROJECT_CUDA_SOURCE )

  source_group( "" FILES ${PROJECT_CUDA_SOURCE} )

  # create the cuda library
  find_package( CUDA REQUIRED )

  # set nvcc options
  set( CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -O3 ${CUDA_COMPUTE_FLAGS}" )
  set( CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} --compiler-options -fno-strict-aliasing -use_fast_math" )


  include_directories( ${PROJECT_CUDA_INCLUDE_DIRS} ${PROJECT_BINARY_DIR} )
  include_directories( SYSTEM ${PROJECT_CUDA_SYSTEM_INCLUDE_DIRS} )


  set( CUDA_LIB cuda${PROJECT_NAME} )

  # build CUDA library
  cuda_add_library     ( ${CUDA_LIB} ${PROJECT_CUDA_SOURCE} )
  target_link_libraries( ${CUDA_LIB} ${CUDA_curand_LIBRARY} )

  set( PROJECT_LINK_LIBS           ${PROJECT_LINK_LIBS}           ${CUDA_LIB} )
  set( PROJECT_INSTALL_TARGETS     ${PROJECT_INSTALL_TARGETS}     ${CUDA_LIB} )
  set( PROJECT_SYSTEM_INCLUDE_DIRS ${PROJECT_SYSTEM_INCLUDE_DIRS} ${CUDA_INCLUDE_DIRS} )

  message( "USING CUDA" )

endif( PROJECT_CUDA_SOURCE )



# compile flags
if ( NOT MSVC )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Wcast-align -Wcast-qual"            )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2"      )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Winit-self -Wmissing-declarations -Wmissing-include-dirs"   )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wold-style-cast -Woverloaded-virtual -Wredundant-decls"     )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=5" )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wswitch-default -Wundef -Werror -Wno-unused"                )
endif( )


# make project into library that can be used by multiple executables ( such as test classes )
add_library            ( ${PROJECT_NAME} ${PROJECT_SOURCE}                   )
target_link_libraries  ( ${PROJECT_NAME} ${PROJECT_LINK_LIBS} ${DEP_TARGETS} )
target_compile_features( ${PROJECT_NAME} PRIVATE cxx_range_for )


if ( ${DEP_TARGETS} )
  add_dependencies ( ${PROJECT_NAME} ${DEP_TARGETS} )
endif( )

target_include_directories( ${PROJECT_NAME} SYSTEM PUBLIC ${PROJECT_SYSTEM_INCLUDE_DIRS} )
target_include_directories( ${PROJECT_NAME}        PUBLIC ${PROJECT_INCLUDE_DIRS}        )


# make executable to run
if ( PROJECT_MAIN )

  set( PROJECT_EXEC run${PROJECT_NAME} )

  add_executable        ( ${PROJECT_EXEC} ${PROJECT_MAIN} )
  target_link_libraries ( ${PROJECT_EXEC} ${PROJECT_NAME} )
  add_dependencies      ( ${PROJECT_EXEC} ${PROJECT_NAME} )

  set_property ( TARGET ${PROJECT_EXEC} PROPERTY CXX_STANDARD 11 ) # c++11

  target_include_directories( ${PROJECT_EXEC} PUBLIC
                              ${PROJECT_INCLUDE_DIRS}
                              ${PROJECT_BINARY_DIR} )

  # Creates a folder "executables" and adds target
  # project (${PROJECT_EXEC}.vcproj) under it
  set_property( TARGET ${PROJECT_EXEC} PROPERTY FOLDER "executables" )

endif()

# Adds logic to INSTALL.vcproj to copy ${PROJECT_EXEC}.exe to destination directory
install(
        TARGETS ${PROJECT_EXEC} ${PROJECT_NAME} ${PROJECT_INSTALL_TARGETS}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )

if ( PROJECT_INSTALL_HEADERS )

  install(
          FILES ${PROJECT_INSTALL_HEADERS}
          DESTINATION include/${PROJECT_NAME}
          )

endif()




# testing
if ( BUILD_TESTS )

  include_directories(${GTEST_INCLUDE_DIRS})

  add_executable( test${PROJECT_NAME} ${TESTING_SOURCE} )

  target_include_directories( test${PROJECT_NAME} SYSTEM PUBLIC ${TESTING_SYSTEM_INCLUDE_DIRS} )
  target_include_directories( test${PROJECT_NAME}        PUBLIC ${TESTING_INCLUDE_DIRS}        )

  target_link_libraries     ( test${PROJECT_NAME} ${TESTING_LINK_LIBS}   )

  if ( ${DEP_TARGETS} )
    add_dependencies ( test${PROJECT_NAME} ${TESTING_DEP_TARGETS} )
  endif( )

endif ( BUILD_TESTS )
