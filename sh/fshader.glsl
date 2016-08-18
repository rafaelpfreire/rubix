#version 130

uniform vec4 aProd;
uniform vec4 dProd;
uniform vec4 sProd;
uniform float shininess;

in  vec3 fN;
in  vec3 fE;
in  vec3 fL;
in  vec4 varyingColor;
out vec4 fragColor;

vec4 Phong(vec3 n)
{
    vec3 N = normalize(n);
    vec3 E = normalize(fE);
    vec3 L = normalize(fL);

    float NdotL = dot(N, L);

    vec3  R  = normalize(2 * NdotL * N - L);
    float Kd = max(NdotL, 0.0);
    float Ks = (NdotL < 0.0) ? 0.0 : pow(max(dot(R, E), 0.0), shininess);

    vec4 diffuse  = Kd * dProd;
    vec4 specular = Ks * sProd;
    vec4 ambient  = aProd;

    return ambient+diffuse+specular;
}

void main(void)
{
    vec4 phongModel = Phong(fN);
    fragColor   = phongModel * varyingColor;
    fragColor.a = 1.0;
}
