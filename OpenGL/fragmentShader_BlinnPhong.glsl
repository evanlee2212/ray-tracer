#version 330 core

in vec4 normal;
in vec3 halfVec;
in vec3 lightDir;

layout(location=0) out vec4 fragmentColor;

uniform vec3 intensity;
uniform vec3 la;
uniform vec3 ka, kd, ks;
uniform float phongExp;

void main(void){
    vec3 n = normalize(normal.xyz);
    vec3 h = normalize(halfVec);
    vec3 l = normalize(lightDir);

    vec3 result = ka * la + kd * intensity * max(0.0, dot(n,
        l)) + ks * intensity * pow(max(0.0, dot(n,h)), phongExp);

    fragmentColor = vec4(result, 1.0);
}