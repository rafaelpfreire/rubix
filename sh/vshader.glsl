#version 130

#define PIECE_LIMIT  0.286
#define WHITE   vec4(1, 1, 1, 1)
#define BLACK   vec4(0, 0, 0, 1)
#define RED     vec4(1, 0, 0, 1)
#define GREEN   vec4(0, 1, 0, 1)
#define BLUE    vec4(0, 0, 1, 1)
#define YELLOW  vec4(0, 1, 1, 1)
#define ORANGE  vec4(1, 1, 0, 1)
#define GRAY    vec4(0.3, 0.3, 0.3, 1)

uniform mat4  mvpMatrix;
uniform vec3  idx;
uniform float distance;

in  vec4 vertex;
in  vec3 normal;
out vec4 varyingColor;

void main(void)
{
    vec4 vPosition = mvpMatrix * (vertex + distance*vec4(idx.x, idx.y, idx.z, 0));
//    float gray = ( vPosition.y + 1 ) * 0.5;

    gl_Position  = vPosition;
//    varyingColor = vec4( gray, gray, gray, 1 );
//    varyingColor = vec4( abs(normal), 1 );

    // Left or Right
    if( vertex.x * idx.x > PIECE_LIMIT )
        if( idx.x > 0 )
            varyingColor = WHITE;
        else
            varyingColor = RED;
    // Up or Down
    else if( vertex.y * idx.y > PIECE_LIMIT )
        if( idx.y > 0 )
            varyingColor = GREEN;
        else
            varyingColor = BLUE;
    // Front or Back
    else if( vertex.z * idx.z > PIECE_LIMIT )
        if( idx.z > 0 )
            varyingColor = YELLOW;
        else
            varyingColor = ORANGE;
    else
        varyingColor = GRAY;
}
