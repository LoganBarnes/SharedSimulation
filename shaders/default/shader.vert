#version 410
#extension GL_ARB_separate_shader_objects : enable


layout( location = 0 ) in vec3 inPosition;
layout( location = 1 ) in vec3 inNormal;
layout( location = 2 ) in vec3 inTexCoords;


uniform mat4 projectionView;
uniform mat4 model;
uniform mat3 normalMat;


layout( location = 0 ) out vec3 position;
layout( location = 1 ) out vec3 normal;
layout( location = 2 ) out vec3 texCoords;


out gl_PerVertex
{
  vec4 gl_Position;
};



void main( void )
{

  position  = vec3( model * vec4( inPosition, 1 ) );
  normal    = normalMat * inNormal;
  texCoords = inTexCoords;

  gl_Position = projectionView * vec4( position, 1.0 );

}
