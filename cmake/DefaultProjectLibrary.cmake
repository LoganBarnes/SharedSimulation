
# Optional variables that can be set before including this file:
#
# PROJECT_CUDA_SOURCE           - string: list of cuda source files to build with nvcc
# USE_CURAND                    - bool:   link the cuRAND cuda library
#
# PROJECT_CONFIG_FILE           - string: hpp file to be filled in by cmake variables
# STRICT_FLAGS                  - bool:   compile using strict gcc/clang compile flags
# PROJECT_SOURCE                - string: list of source files to build with C++ compiler
# PROJECT_MAIN                  - string: file with main() function
# PROJECT_INSTALL_HEADER_DIRS   - string: directories to install in 'include' folder
# BUILD_TESTS                   - bool:   build unit tests
# TESTING_SOURCE                - string: list of unit test source files
#


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

# targets that must be built before project lib
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


# Create named folders for the sources within the .vcproj
# Empty name lists them directly under the .vcproj
source_group( "" FILES ${PROJECT_SOURCE}      )


# create a cuda lib if necessary
if ( PROJECT_CUDA_SOURCE )

  source_group( "" FILES ${PROJECT_CUDA_SOURCE} )

  set(
      PROJECT_CUDA_INCLUDE_DIRS
      ${PROJECT_CUDA_INCLUDE_DIRS}

      ${SHARED_INCLUDE_DIRS}
      ${SHARED_CUDA_INCLUDE_DIRS}
      ${SHARED_PATH}/src/cuda
      )

  set(
      PROJECT_CUDA_SYSTEM_INCLUDE_DIRS
      ${PROJECT_CUDA_SYSTEM_INCLUDE_DIRS}

      ${SHARED_SYSTEM_INCLUDE_DIRS}
      ${SHARED_CUDA_SYSTEM_INCLUDE_DIRS}
      ${SHARED_PATH}/src/cuda/helpers
      )

  set(
      PROJECT_CUDA_LINK_LIBS
      ${PROJECT_CUDA_LINK_LIBS}
      ${SHARED_CUDA_LINK_LIBS}
      )

  set(
      PROJECT_CUDA_DEP_TARGETS
      ${PROJECT_CUDA_DEP_TARGETS}
      ${SHARED_CUDA_DEP_TARGETS}
      )

  # create the cuda library
  find_package( CUDA REQUIRED )

  # set nvcc options
  if( UNIX OR APPLE )
    list( APPEND CUDA_NVCC_FLAGS
         -Xcompiler -fPIC -O3 --compiler-options -fno-strict-aliasing -use_fast_math )
  endif( )

  if( APPLE )
    list( APPEND CUDA_NVCC_FLAGS -Xcompiler -Wno-unused-function)
  endif( )

  # Auto detect compute architecture
  if ( NOT ${CMAKE_VERSION} VERSION_LESS 3.7.1 AND NOT CUDA_ARCH_FLAGS )
    cuda_select_nvcc_arch_flags( CUDA_ARCH_FLAGS Auto )
    set( CUDA_ARCH_FLAGS ${CUDA_ARCH_FLAGS} CACHE STRING "gencode architecture flags for nvcc" )
  endif( )

  list( APPEND CUDA_NVCC_FLAGS ${CUDA_ARCH_FLAGS} --std=c++${FORCE_CUDA_STANDARD} )


  include_directories( ${PROJECT_CUDA_INCLUDE_DIRS} ${PROJECT_BINARY_DIR} )
  include_directories( SYSTEM ${PROJECT_CUDA_SYSTEM_INCLUDE_DIRS} )


  set( CUDA_LIB cuda${PROJECT_NAME} )

  # build CUDA library
  cuda_add_library( ${CUDA_LIB} ${PROJECT_CUDA_SOURCE} )

  if ( USE_CURAND )
    target_link_libraries( ${CUDA_LIB} ${CUDA_curand_LIBRARY} )
  endif( )

  target_link_libraries( ${CUDA_LIB} ${PROJECT_CUDA_LINK_LIBS} )

  if ( FORCE_CPP_STANDARD )
    set_property( TARGET ${CUDA_LIB} PROPERTY CXX_STANDARD ${FORCE_CPP_STANDARD} )
    set_property( TARGET ${CUDA_LIB} PROPERTY CXX_STANDARD_REQUIRED ON )
  else()
    target_compile_features( ${CUDA_LIB} PRIVATE cxx_range_for )
  endif()

  set( PROJECT_LINK_LIBS           ${PROJECT_LINK_LIBS}           ${CUDA_LIB} )
  set( PROJECT_INSTALL_TARGETS     ${PROJECT_INSTALL_TARGETS}     ${CUDA_LIB} )
  set( PROJECT_SYSTEM_INCLUDE_DIRS ${PROJECT_SYSTEM_INCLUDE_DIRS} ${CUDA_INCLUDE_DIRS} )

  message( "USING CUDA" )

  if ( PTX_SOURCE )

    set( CUDA_GENERATED_OUTPUT_DIR ${RES_PATH}/ptx )
    cuda_wrap_srcs( cuda${PROJECT_NAME} PTX GENERATED_PTX ${PTX_SOURCE} )

    set( PROJECT_SOURCE ${PROJECT_SOURCE} ${GENERATED_PTX} )

  endif( )

  set( USE_CUDA ON )

endif( PROJECT_CUDA_SOURCE )



if ( PROJECT_CONFIG_FILE )

  set ( PROJECT_INCLUDE_DIRS ${PROJECT_INCLUDE_DIRS} ${PROJECT_BINARY_DIR} )

  configure_file (
                  ${PROJECT_CONFIG_FILE}
                  ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.hpp
                  )

endif( PROJECT_CONFIG_FILE )


# compile flags
if ( NOT MSVC AND STRICT_FLAGS )
  list(
       APPEND INTENSE_FLAGS
       -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -fPIC -Wctor-dtor-privacy
       -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-declarations -Wundef -Werror
       -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls
       -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=5 -Wswitch-default -Wno-unused
       )
endif( )


# make project into library that can be used by multiple executables ( such as test classes )
if ( PROJECT_SOURCE )

  set( PROJECT_LIB ${PROJECT_NAME} )

  add_library            ( ${PROJECT_LIB} ${PROJECT_SOURCE}    )
  target_link_libraries  ( ${PROJECT_LIB} ${PROJECT_LINK_LIBS} )

  if ( FORCE_CPP_STANDARD )
    set_property( TARGET ${PROJECT_LIB} PROPERTY CXX_STANDARD ${FORCE_CPP_STANDARD} )
    set_property( TARGET ${PROJECT_LIB} PROPERTY CXX_STANDARD_REQUIRED ON )
  else()
    target_compile_features( ${PROJECT_LIB} PRIVATE cxx_range_for )
  endif()

  if ( INTENSE_FLAGS )
    set_target_properties( ${PROJECT_LIB} PROPERTIES COMPILE_FLAGS ${INTENSE_FLAGS} )
  endif( )

  if ( PROJECT_DEP_TARGETS )
    add_dependencies ( ${PROJECT_LIB} ${PROJECT_DEP_TARGETS} )
  endif( )

  target_include_directories( ${PROJECT_LIB} SYSTEM PUBLIC ${PROJECT_SYSTEM_INCLUDE_DIRS} )
  target_include_directories( ${PROJECT_LIB}        PUBLIC ${PROJECT_INCLUDE_DIRS}        )

endif( PROJECT_SOURCE )



# make executable to run
if ( PROJECT_MAIN )

  set( PROJECT_EXEC run${PROJECT_NAME} )

  add_executable ( ${PROJECT_EXEC} ${PROJECT_MAIN} )

  if ( PROJECT_LIB )
    target_link_libraries ( ${PROJECT_EXEC} ${PROJECT_LIB} ${PROJECT_LINK_LIBS} )
    add_dependencies      ( ${PROJECT_EXEC} ${PROJECT_LIB} )
  else( )
    target_link_libraries ( ${PROJECT_EXEC} ${PROJECT_LINK_LIBS} )
    if ( PROJECT_DEP_TARGETS )
      add_dependencies ( ${PROJECT_EXEC} ${PROJECT_DEP_TARGETS} )
    endif()
  endif( )

  if ( FORCE_CPP_STANDARD )
    set_property( TARGET ${PROJECT_EXEC} PROPERTY CXX_STANDARD ${FORCE_CPP_STANDARD} )
    set_property( TARGET ${PROJECT_EXEC} PROPERTY CXX_STANDARD_REQUIRED ON )
  else()
    target_compile_features( ${PROJECT_EXEC} PRIVATE cxx_range_for )
  endif()

  if ( INTENSE_FLAGS )
    set_target_properties( ${PROJECT_EXEC} PROPERTIES COMPILE_FLAGS ${INTENSE_FLAGS} )
  endif( )

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

if ( PROJECT_INSTALL_HEADER_DIRS )

  install(
          DIRECTORY ${PROJECT_INSTALL_HEADER_DIRS}
          DESTINATION include
          )

endif()




# testing
if ( BUILD_TESTS AND TESTING_SOURCE )

  set( PROJECT_UNIT_TESTS test${PROJECT_NAME} )

  add_executable( ${PROJECT_UNIT_TESTS} ${TESTING_SOURCE} )

  target_include_directories( ${PROJECT_UNIT_TESTS} SYSTEM PUBLIC ${TESTING_SYSTEM_INCLUDE_DIRS} )
  target_include_directories( ${PROJECT_UNIT_TESTS}        PUBLIC ${TESTING_INCLUDE_DIRS}        )

  target_link_libraries( ${PROJECT_UNIT_TESTS} ${TESTING_LINK_LIBS} gmock gmock_main ${PROJECT_LIB} )

  if ( FORCE_CPP_STANDARD )
    set_property( TARGET ${PROJECT_UNIT_TESTS} PROPERTY CXX_STANDARD ${FORCE_CPP_STANDARD} )
    set_property( TARGET ${PROJECT_UNIT_TESTS} PROPERTY CXX_STANDARD_REQUIRED ON )
  else()
    target_compile_features( ${PROJECT_UNIT_TESTS} PRIVATE cxx_range_for )
  endif()

  if ( INTENSE_FLAGS )
    set_target_properties( ${PROJECT_UNIT_TESTS} PROPERTIES COMPILE_FLAGS ${INTENSE_FLAGS} )
  endif( )

  if ( ${DEP_TARGETS} )
    add_dependencies ( ${PROJECT_UNIT_TESTS} ${TESTING_DEP_TARGETS} gmock gmock_main ${PROJECT_LIB} )
  endif( )

  # set_property( TARGET ${PROJECT_UNIT_TESTS} PROPERTY INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib )

  # Adds logic to INSTALL.vcproj to copy ${PROJECT_EXEC}.exe to destination directory
  install(
          TARGETS ${PROJECT_UNIT_TESTS}
          RUNTIME DESTINATION testbin
          )

  # Set up cmake testing
  enable_testing()
  include( CTest )

  foreach( testFile ${TESTING_SOURCE} )

    # remove '.cpp' and full path from test name
    string( REPLACE ".cpp" "" testName ${testFile} )
    string( REPLACE "${SRC_DIR}/testing/" "" testName ${testName} )

    add_test( NAME ${testName} COMMAND ${PROJECT_UNIT_TESTS} "--gtest_filter=${testName}*" )

  endforeach( )

endif ( BUILD_TESTS AND TESTING_SOURCE )
