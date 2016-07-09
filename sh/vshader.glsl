#version 130

uniform mat4 mvpMatrix;

in  vec4 vertex;
in  vec3 normal;
out vec4 varyingColor;

void main(void)
{
    vec4 x_norm = vec4(1,0,0,0);
    vec4 r_pos  = vec4(0.5,0,0,1);

    vec4 vPosition = mvpMatrix * vertex;
    vec4 xNorm     = mvpMatrix * x_norm;
    vec4 rPos      = mvpMatrix * r_pos;
    float gray = ( vPosition.y + 1 ) * 0.5;

    gl_Position  = vPosition;
//    varyingColor = vec4( gray, gray, gray, 1 );
//    varyingColor = vec4( abs(normal).x, 1, 1, 1 );

    float dot1 = dot(xNorm, rPos);
    float dot2 = dot(xNorm, vPosition);

    if( abs(vertex.x) > 0.286 )
        varyingColor = vec4(1,0,0,1);
    else if( abs(vertex.y) > 0.286 )
        varyingColor = vec4(0,1,0,1);
    else if( abs(vertex.z) > 0.286 )
        varyingColor = vec4(0,0,1,1);
    else
        varyingColor = vec4(0,0,0,1);
}
