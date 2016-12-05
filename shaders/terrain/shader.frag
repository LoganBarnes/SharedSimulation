#version 450

layout( location = 0 ) in vec3 screenPos;


layout( location = 0 ) out vec4 outColor;


void main( void )
{

  outColor = vec4( screenPos, 1.0 );

}
