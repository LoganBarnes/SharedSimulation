#version 410

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec3 normal;
layout( location = 2 ) in vec3 texCoords;


uniform vec3 color;


layout( location = 0 ) out vec4 outColor;


void main( void )
{

  outColor = vec4( color, 1.0 );

}
