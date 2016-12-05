@ECHO OFF

rem goes into each shader directory and comiles the shaders into
rem SPIR-V byte code

rem iterate over directories
for /D %%d in (*) do (

  rem go into current dir
  cd %%d

  echo Current directory: %%d

  rem iterate over shader files only
  for %%f in ("shader.*") do (

    rem compile shader into SPIR-V byte code
    glslangValidator.exe -V %%f

  )

  rem exit current directory
  cd ..

)
