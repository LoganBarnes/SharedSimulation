#version 410
#extension GL_ARB_separate_shader_objects : enable


layout( location = 0 ) in vec3 inPosition;


uniform mat4 projectionViewModel = mat4( 1.0 );


out gl_PerVertex
{
  vec4 gl_Position;
};



void main( void )
{

  gl_Position = projectionViewModel * vec4( inPosition, 1.0 );

}
