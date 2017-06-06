#version 410

uniform vec3 color;

layout( location = 0 ) out vec4 outColor;

void main( void )
{
  vec3 norm;
  norm.xy = gl_PointCoord * vec2( 2.0, -2.0 ) + vec2( -1.0, 1.0 );
  float magPow2 = dot( norm.xy, norm.xy );

  if ( magPow2 > 1.0 ) discard; // kill pixels outside circle

  outColor = vec4( color, 1.0 );
}
