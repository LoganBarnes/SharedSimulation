#version 410


uniform sampler2D tex;
uniform vec2      screenSize;
uniform vec2      lowerLeft = vec2( 0.0 );

uniform int redOnly = 0;

layout( location = 0 ) out vec4 outColor;


void main( void )
{
  outColor = texture( tex, ( gl_FragCoord.xy - lowerLeft ) / screenSize );

  if ( redOnly == 1 )
  {
    outColor.rgb = outColor.rrr;
  }
}
