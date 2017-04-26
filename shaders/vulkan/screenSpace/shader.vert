#version 450
#extension GL_ARB_separate_shader_objects : enable


//layout( location = 0 ) in vec3 inScreenPos;


layout( location = 0 ) out vec3 screenPos;


out gl_PerVertex
{
  vec4 gl_Position;
};


// temporary
vec3 positions[ 4 ] = vec3[] (
  vec3( -1.0, -1.0, 0.0 ),
  vec3( -1.0,  1.0, 0.0 ),
  vec3(  1.0, -1.0, 0.0 ),
  vec3(  1.0,  1.0, 0.0 )
);


void main( void )
{

  screenPos = positions[ gl_VertexIndex ];

  gl_Position = vec4( screenPos, 1.0 );

}
