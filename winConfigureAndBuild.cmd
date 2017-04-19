@ECHO OFF

rem create the build directory if it doesn't exist
cmake -E make_directory build

rem run cmake from the build directory to configure the project
cmake -E chdir build cmake -DCMAKE_GENERATOR_PLATFORM=x64 ^
                           -DBUILD_SHARED_TESTS=ON ^
                           -DUSE_GLM=ON ^
                           -DUSE_GLFW=ON ^
                           -DUSE_GUI=ON ^
                           ..

rem run the cmake build command to build the project with the native build system
cmake -E chdir build cmake --build . --config Release -- /m
