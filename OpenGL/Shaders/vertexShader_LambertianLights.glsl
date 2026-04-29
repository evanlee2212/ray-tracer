#version 410

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

#define MAX_LIGHTS 8
uniform int  numLights;
uniform vec4 lightPosWorld[MAX_LIGHTS];

out vec3 fragNormal;
out vec3 lightDir[MAX_LIGHTS];

void main(void)
{
    vec4 pos   = viewMatrix * modelMatrix * vec4(in_Position, 1.0);
    fragNormal = vec3(normalMatrix * vec4(in_Normal, 0.0));

    for (int i = 0; i < numLights; i++) {
        vec3 lp     = vec3(viewMatrix * lightPosWorld[i]);
        lightDir[i] = normalize(lp - pos.xyz);
    }

    gl_Position = projMatrix * pos;
}