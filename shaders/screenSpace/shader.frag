#version 410


uniform sampler2D tex;
uniform vec2      screenSize;


layout( location = 0 ) out vec4 outColor;


void main( void )
{
  outColor = texture( tex, gl_FragCoord.xy / screenSize );
}
