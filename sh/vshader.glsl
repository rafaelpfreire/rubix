#version 130

uniform mat4 mvpMatrix;

in  vec4 vertex;
in  vec3 normal;
//in  vec4 color;
out vec4 varyingColor;

void main(void)
{
    vec4 vPosition = mvpMatrix * vertex;
    float gray = ( vPosition.y + 1 ) * 0.5;

    gl_Position  = vPosition;
    varyingColor = vec4( gray, gray, gray, 1 );
//    varyingColor = vec4( abs(normal), 1 );
}
