#version 410

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;
uniform mat4 lightSpaceMatrix;

#define MAX_LIGHTS 8
uniform int  numLights;
uniform vec4 lightPosWorld[MAX_LIGHTS];

out vec3 fragNormal;
out vec3 fragPos;
out vec4 fragPosLightSpace;

void main()
{
    vec4 worldPos = modelMatrix * vec4(in_Position, 1.0);
    vec4 pos      = viewMatrix  * worldPos;

    fragNormal        = vec3(normalMatrix * vec4(in_Normal, 0.0));
    fragPos           = pos.xyz;
    fragPosLightSpace = lightSpaceMatrix * worldPos;

    gl_Position = projMatrix * pos;
}
