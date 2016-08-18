#version 130

uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;
uniform mat3 nMatrix;

uniform vec3 idx;
uniform vec4 rFace[4];
uniform vec4 lFace[4];
uniform vec4 uFace[4];
uniform vec4 dFace[4];
uniform vec4 fFace[4];
uniform vec4 bFace[4];
uniform vec4 lightPosition;

in  vec4 vertex;
in  vec3 normal;
out vec3 fN;
out vec3 fL;
out vec3 fV;
out vec4 varyingColor;

const vec4 WHITE  = vec4(1, 1, 1, 1);
const vec4 BLACK  = vec4(0, 0, 0, 1);
const vec4 RED    = vec4(1, 0, 0, 1);
const vec4 GREEN  = vec4(0, 1, 0, 1);
const vec4 BLUE   = vec4(0, 0, 1, 1);
const vec4 ORANGE = vec4(0.9, 0.5, 0, 1);
const vec4 YELLOW = vec4(1, 1, 0, 1);
const vec4 GRAY   = vec4(0.2, 0.2, 0.2, 1);

void main(void)
{
    // Camera Position
    vec4 eyePosition = vMatrix * mMatrix * vertex;

    // Vertex Position
    gl_Position  = pMatrix * eyePosition;

    // Phong Model Vectors
    fN = mat3(vMatrix) * nMatrix * normal;
    fL = lightPosition.xyz - eyePosition.xyz;
    fV = -eyePosition.xyz;

    // Color
    varyingColor = GRAY;
    for( int i = 0; i < 4; i++ ) {
        if( vertex == rFace[i] && idx.x == 1.0 )
            varyingColor = ORANGE;
        else if( vertex == lFace[i] && idx.x == -1.0 )
            varyingColor = RED;
        else if( vertex == uFace[i] && idx.y ==  1.0 )
            varyingColor = YELLOW;
        else if( vertex == dFace[i] && idx.y == -1.0 )
            varyingColor = WHITE;
        else if( vertex == fFace[i] && idx.z ==  1.0 )
            varyingColor = GREEN;
        else if( vertex == bFace[i] && idx.z == -1.0 )
            varyingColor = BLUE;
    }
}
