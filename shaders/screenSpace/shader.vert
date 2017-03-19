#version 410
#extension GL_ARB_separate_shader_objects : enable


layout( location = 0 ) in vec3 inScreenPos;


layout( location = 0 ) out vec3 screenPos;


out gl_PerVertex
{
  vec4 gl_Position;
};


void main( void )
{
  screenPos = inScreenPos;

  gl_Position = vec4( screenPos, 1.0 );
}
