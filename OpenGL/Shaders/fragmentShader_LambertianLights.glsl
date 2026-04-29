#version 410

#define MAX_LIGHTS 8

in vec3 fragNormal;
in vec3 lightDir[MAX_LIGHTS];

uniform int   numLights;
uniform vec3  intensity[MAX_LIGHTS];
uniform vec3  diffuseComponent;

out vec4 fragmentColor;

void main(void)
{
    vec3 N = normalize(fragNormal);

    vec3 color = vec3(0.0);
    for (int i = 0; i < numLights; i++) {
        vec3 L = normalize(vec3(lightDir[i]));
        color += diffuseComponent * intensity[i] * max(0.0, dot(N, L));
    }

    fragmentColor = vec4(color, 1.0);
}
