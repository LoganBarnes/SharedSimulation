#version 410 core

layout( location = 0 ) in vec2 texCoords;

uniform sampler2D tex;
uniform int       redOnly = 0;

layout( location = 0 ) out vec4 outColor;

void main( void )
{
  outColor = texture( tex, texCoords );

  if ( redOnly == 1 )
  {
    outColor.rgb = outColor.rrr;
  }
}
