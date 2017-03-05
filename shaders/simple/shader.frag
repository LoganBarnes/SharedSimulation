#version 410


uniform vec3 color;


layout( location = 0 ) out vec4 outColor;


void main( void )
{

  outColor = vec4( color, 1.0 );

}
