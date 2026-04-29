#version 410

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_TexCoord;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

#define MAX_LIGHTS 8
uniform int numLights;
uniform vec4 lightPosWorld[MAX_LIGHTS];

out vec3 fragNormal;
out vec3 fragPos;
out vec2 texCoord;

out vec3 lightDir[MAX_LIGHTS];
out vec3 halfVec[MAX_LIGHTS];

void main(void)
{
    vec4 pos      = viewMatrix * modelMatrix * vec4(in_Position, 1.0);
    vec3 viewDir  = normalize(-pos.xyz);

    fragNormal = vec3(normalMatrix * vec4(in_Normal, 0.0));
    fragPos    = pos.xyz;
    texCoord   = in_TexCoord;

    for (int i = 0; i < numLights; i++) {
        vec3 lp      = vec3(viewMatrix * lightPosWorld[i]);
        lightDir[i]  = normalize(lp - pos.xyz);
        halfVec[i]   = normalize(lightDir[i] + viewDir);
    }

    gl_Position = projMatrix * pos;
}