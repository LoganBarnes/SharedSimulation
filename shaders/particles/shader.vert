#version 410
#extension GL_ARB_separate_shader_objects : enable

layout( location = 0 ) in vec3 inPosition;

uniform int   screenHeight;
uniform mat4  projectionView;
uniform mat4  projection;
uniform float particleRadius = 0.1;

void main( )
{
  gl_Position  = projectionView * vec4( inPosition, 1.0 );
  gl_PointSize = float( screenHeight ) * projection[ 1 ][ 1 ] * particleRadius / gl_Position.w;
}