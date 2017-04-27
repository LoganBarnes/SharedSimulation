#!/bin/bash

# example usage:
# ./unixConfigureAndBuild.sh -sf -gl -x -i -m -t

# toggleable variables
BUILD_MODE=Release
STRICT_FLAGS=OFF
USE_GLFW=OFF
USE_VULKAN=OFF
USE_GLM=OFF
USE_OPTIX=OFF
USE_GUI=OFF
USE_GMOCK=OFF
TESTING=OFF
SHARED_LIBS=OFF

CLEAN=false


function printUsage() {
  echo "Usage: ./unixConfigureAndBuild.sh <options>";
  echo "    options:";
  echo "        -sf or --strict-flags -> enable strict compile flags";
  echo "        -gl or --glfw         -> download and build the glfw library";
  echo "        -v  or --vulkan       -> download and build the vulkan library";
  echo "        -m  or --glm          -> download and build the glm library";
  echo "        -x  or --optix        -> download and build the optix library";
  echo "        -i  or --imgui        -> download and build the imgui library";
  echo "        -t  or --test         -> compile unit tests";
  echo "        -d  or --debug        -> compile in debug mode";
  echo "        -s  or --shared       -> compile as shared libraries";
  echo "        -c  or --clean        -> clean project (delete build folder)";
  echo "        -h  or --help         -> print this message";
}

# http://stackoverflow.com/questions/192249/how-do-i-parse-command-line-arguments-in-bash

# Use -gt 1 to consume two arguments per pass in the loop (e.g. each
# argument has a corresponding value to go with it).
# Use -gt 0 to consume one or more arguments per pass in the loop (e.g.
# some arguments don't have a corresponding value to go with it such
# as in the --default example).
# note: if this is set to -gt 0 the /etc/hosts part is not recognized ( may be a bug )
while [[ $# -gt 0 ]]
do
key="$1"

case $key in
    -sf|--strict-flags)
    STRICT_FLAGS=ON
    ;;
    -gl|--glfw)
    USE_GLFW=ON
    ;;
    -v|--vulkan)
    USE_VULKAN=ON
    ;;
    -m|--glm)
    USE_GLM=ON
    ;;
    -x|--optix)
    USE_OPTIX=ON
    ;;
    -i|--imgui)
    USE_GUI=ON
    ;;
    -t|--test)
    TESTING=ON
    ;;
    -s|--shared)
    SHARED_LIBS=ON
    ;;
    -d|--debug)
    BUILD_MODE=Debug
    ;;
    -c|--clean)
    CLEAN=true
    ;;
    -h|--help)
    printUsage;
    [[ "${BASH_SOURCE[0]}" != "${0}" ]] && return 0 || exit 0
    ;;
    *)
    echo "Unrecognized option: '$key'" # unknown option
    printUsage;
    [[ "${BASH_SOURCE[0]}" != "${0}" ]] && return 1 || exit 1
    ;;
esac
shift # past argument or value
done


# clean project by removing build dir
if [[ "$CLEAN" == true ]]
  then

  cmake -E remove_directory build
  echo "Project clean."
  [[ "${BASH_SOURCE[0]}" != "${0}" ]] && return 0 || exit 0
fi;


# current run directory
ROOT_DIR=$(pwd)

# create the build directory
cmake -E make_directory build

# run cmake from the build directory to configure the project
cmake -E chdir build cmake -DCMAKE_BUILD_TYPE=$BUILD_MODE \
                           -DSTRICT_FLAGS=$STRICT_FLAGS \
                           -DUSE_GLFW=$USE_GLFW \
                           -DUSE_VULKAN=$USE_VULKAN \
                           -DUSE_GLM=$USE_GLM \
                           -DUSE_OPTIX=$USE_OPTIX \
                           -DUSE_GUI=$USE_GUI \
                           -DUSE_GMOCK=$USE_GMOCK \
                           -DBUILD_SHARED_TESTS=$TESTING \
                           -DBUILD_SHARED_LIBS=$SHARED_LIBS \
                           -DCMAKE_INSTALL_PREFIX=$ROOT_DIR/build ..

# run the cmake build command to build the project with the native build system
cmake -E chdir build cmake --build . --target install --config $BUILD_MODE -- -j12
