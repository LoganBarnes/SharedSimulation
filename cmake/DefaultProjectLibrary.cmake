
if ( PROJECT_CONFIG_FILE )

  configure_file (
                  ${PROJECT_CONFIG_FILE}
                  ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.hpp
                  )

endif( ) # PROJECT_CONFIG_FILE


# system header dirs
set(
    PROJECT_SYSTEM_INCLUDE_DIRS
    ${PROJECT_SYSTEM_INCLUDE_DIRS}
    ${SHARED_SYSTEM_INCLUDE_DIRS}
    )

# libraries to link against
set(
    PROJECT_LINK_LIBS
    ${PROJECT_LINK_LIBS}
    ${SHARED_LINK_LIBS}
    )

# must be built before project lib
set(
    PROJECT_DEP_TARGETS
    ${PROJECT_DEP_TARGETS}
    ${SHARED_DEP_TARGETS}
    )

# header dirs
set(
    PROJECT_INCLUDE_DIRS
    ${PROJECT_INCLUDE_DIRS}
    ${SHARED_INCLUDE_DIRS}
    )

# cpp files
set(
    PROJECT_SOURCE
    ${PROJECT_SOURCE}
    ${SHARED_SOURCE}
    )


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
  if ( NOT MSVC )
    set( CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -O3 ${CUDA_COMPUTE_FLAGS} -Wno-deprecated-gpu-targets" )
    set( CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} --compiler-options -fno-strict-aliasing -use_fast_math" )
  else()
    set( CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} ${CUDA_COMPUTE_FLAGS} -Wno-deprecated-gpu-targets" )
  endif()


  include_directories( ${PROJECT_CUDA_INCLUDE_DIRS} ${PROJECT_BINARY_DIR} )
  include_directories( SYSTEM ${PROJECT_CUDA_SYSTEM_INCLUDE_DIRS} )


  set( CUDA_LIB cuda${PROJECT_NAME} )

  # build CUDA library
  cuda_add_library( ${CUDA_LIB} ${PROJECT_CUDA_SOURCE} )

  if ( USE_CURAND )
    target_link_libraries( ${CUDA_LIB} ${CUDA_curand_LIBRARY} )
  endif( )

  target_link_libraries( ${CUDA_LIB} ${PROJECT_CUDA_LINK_LIBS} )

  set( PROJECT_LINK_LIBS           ${PROJECT_LINK_LIBS}           ${CUDA_LIB} )
  set( PROJECT_INSTALL_TARGETS     ${PROJECT_INSTALL_TARGETS}     ${CUDA_LIB} )
  set( PROJECT_SYSTEM_INCLUDE_DIRS ${PROJECT_SYSTEM_INCLUDE_DIRS} ${CUDA_INCLUDE_DIRS} )

  message( "USING CUDA" )

  if ( PTX_SOURCE )

    set( CUDA_GENERATED_OUTPUT_DIR ${RES_PATH}/ptx )
    cuda_wrap_srcs( cuda${PROJECT_NAME} PTX GENERATED_PTX ${PTX_SOURCE} )

    set( PROJECT_SOURCE ${PROJECT_SOURCE} ${GENERATED_PTX} )

  endif( )

endif( PROJECT_CUDA_SOURCE )



# compile flags
if ( NOT MSVC AND STRICT_FLAGS )
  set( INTENSE_FLAGS "${INTENSE_FLAGS} -pedantic -Wall -Wextra -Wcast-align -Wcast-qual"            )
  set( INTENSE_FLAGS "${INTENSE_FLAGS} -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2"      )
  set( INTENSE_FLAGS "${INTENSE_FLAGS} -Winit-self -Wmissing-declarations -Wmissing-include-dirs"   )
  set( INTENSE_FLAGS "${INTENSE_FLAGS} -Wold-style-cast -Woverloaded-virtual -Wredundant-decls"     )
  set( INTENSE_FLAGS "${INTENSE_FLAGS} -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=5" )
  set( INTENSE_FLAGS "${INTENSE_FLAGS} -Wswitch-default -Wundef -Werror -Wno-unused"                )
endif( )

set( STRICT_DEBUG_FLAGS "${INTENSE_FLAGS}" )
set( STRICT_RELEASE_FLAGS "${INTENSE_FLAGS} -O3" )


# make project into library that can be used by multiple executables ( such as test classes )
if ( PROJECT_SOURCE )

  set( PROJECT_LIB ${PROJECT_NAME} )

  add_library            ( ${PROJECT_LIB} ${PROJECT_SOURCE}                   )
  target_link_libraries  ( ${PROJECT_LIB} ${PROJECT_LINK_LIBS} ${DEP_TARGETS} )
  target_compile_features( ${PROJECT_LIB} PRIVATE cxx_range_for               )

  target_compile_options ( ${PROJECT_LIB} PUBLIC "$<$<CONFIG:DEBUG>:${INTENSE_DEBUG_FLAGS}>")
  target_compile_options ( ${PROJECT_LIB} PUBLIC "$<$<CONFIG:RELEASE>:${INTENSE_RELEASE_FLAGS}>")


  if ( ${DEP_TARGETS} )
    add_dependencies ( ${PROJECT_LIB} ${DEP_TARGETS} )
  endif( )

  target_include_directories( ${PROJECT_LIB} SYSTEM PUBLIC ${PROJECT_SYSTEM_INCLUDE_DIRS} )
  target_include_directories( ${PROJECT_LIB}        PUBLIC ${PROJECT_INCLUDE_DIRS}        )

endif( PROJECT_SOURCE )



# make executable to run
if ( PROJECT_MAIN )

  set( PROJECT_EXEC run${PROJECT_NAME} )

  add_executable ( ${PROJECT_EXEC} ${PROJECT_MAIN} )

  if ( PROJECT_LIB )
    target_link_libraries ( ${PROJECT_EXEC} ${PROJECT_LIB} )
    add_dependencies      ( ${PROJECT_EXEC} ${PROJECT_LIB} )
  endif( )

  target_compile_features( ${PROJECT_EXEC} PRIVATE cxx_range_for )

  target_compile_options ( ${PROJECT_EXEC} PUBLIC "$<$<CONFIG:DEBUG>:${INTENSE_DEBUG_FLAGS}>")
  target_compile_options ( ${PROJECT_EXEC} PUBLIC "$<$<CONFIG:RELEASE>:${INTENSE_RELEASE_FLAGS}>")

  target_include_directories( ${PROJECT_EXEC} PUBLIC
                              ${PROJECT_INCLUDE_DIRS}
                              ${PROJECT_BINARY_DIR} )

  # Creates a folder "executables" and adds target
  # project (${PROJECT_EXEC}.vcproj) under it
  set_property( TARGET ${PROJECT_EXEC} PROPERTY FOLDER "executables" )

endif()

# rpath for unix libs
set_property( TARGET ${PROJECT_EXEC} PROPERTY INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib )

# Adds logic to INSTALL.vcproj to copy ${PROJECT_EXEC}.exe to destination directory
install(
        TARGETS ${PROJECT_EXEC} ${PROJECT_LIB} ${PROJECT_INSTALL_TARGETS}
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

  include_directories( ${GTEST_INCLUDE_DIRS} )

  set( PROJECT_UNIT_TESTS test${PROJECT_NAME} )

  add_executable( PROJECT_UNIT_TEST ${TESTING_SOURCE} )

  target_include_directories( PROJECT_UNIT_TEST SYSTEM PUBLIC ${TESTING_SYSTEM_INCLUDE_DIRS} )
  target_include_directories( PROJECT_UNIT_TEST        PUBLIC ${TESTING_INCLUDE_DIRS}        )

  target_link_libraries( PROJECT_UNIT_TEST ${TESTING_LINK_LIBS}   )

  target_compile_features( PROJECT_UNIT_TEST PRIVATE cxx_range_for )

  if ( ${DEP_TARGETS} )

    add_dependencies ( PROJECT_UNIT_TEST ${TESTING_DEP_TARGETS} )

  endif( )

  # Adds logic to INSTALL.vcproj to copy ${PROJECT_EXEC}.exe to destination directory
  install(
          TARGETS PROJECT_UNIT_TEST
          RUNTIME DESTINATION testbin
          )

endif ( BUILD_TESTS )
