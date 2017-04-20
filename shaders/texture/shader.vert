#version 410 core
#extension GL_ARB_separate_shader_objects : enable


layout( location = 0 ) in vec3 inScreenPos;


layout( location = 0 ) out vec2 texCoords;


out gl_PerVertex
{
  vec4 gl_Position;
};


void main( void )
{
  texCoords = ( inScreenPos.xy * 0.5 ) + vec2( 0.5 );

  gl_Position = vec4( inScreenPos, 1.0 );
}
