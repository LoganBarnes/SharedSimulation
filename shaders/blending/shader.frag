#version 410


uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec2      texSize;
uniform float     alpha;


layout( location = 0 ) out vec4 outColor;


void main( void )
{

  vec2 texCoords = gl_FragCoord.xy / texSize;

  vec3 color1 = texture( texture1, texCoords ).rgb;
  vec3 color2 = texture( texture2, texCoords ).rgb;

  vec3 color = color1 * alpha + color2 * ( 1.0 - alpha );

  outColor = vec4( color, 1.0 );

}
