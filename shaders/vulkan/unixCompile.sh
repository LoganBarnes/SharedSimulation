#!/bin/bash

# goes into each shader directory and comiles
# the shaders into SPIR-V byte code

# iterate over directories
for SHADER_DIR in $(pwd)/*; do

  [ -d "${SHADER_DIR}" ] || continue # if not a directory, skip

  # go into current dir
  cd ${SHADER_DIR}

  echo "Current directory: $(basename "${SHADER_DIR}")"

  # iterate over shader files only
  for SHADER_FILE in ${SHADER_DIR}/shader.*; do

    # compile shader into SPIR-V byte code
    glslangValidator -V "$(basename "${SHADER_FILE}")"

  done

  # exit current directory
  cd ..

done

