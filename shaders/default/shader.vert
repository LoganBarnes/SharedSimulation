#version 410
#extension GL_ARB_separate_shader_objects : enable


layout( location = 0 ) in vec3 inPosition;
layout( location = 1 ) in vec3 inColor;


uniform mat4 projectionViewModel;


layout( location = 0 ) out vec3 fragColor;


out gl_PerVertex
{
  vec4 gl_Position;
};



void main( void )
{

  gl_Position = projectionViewModel * vec4( inPosition, 1.0 );
  fragColor   = inColor;

}
