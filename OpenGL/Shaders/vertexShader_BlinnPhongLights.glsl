#version 410

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_TexCoord;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

#define MAX_LIGHTS 8
uniform int  numLights;
uniform vec4 lightPosWorld[MAX_LIGHTS];

// One light-space matrix per shadow-casting light
uniform mat4 lightSpaceMatrix;

out vec3 fragNormal;
out vec3 fragPos;        // view-space
out vec2 texCoord;
out vec4 fragPosLightSpace;  // for shadow lookup

void main()
{
    vec4 worldPos = modelMatrix * vec4(in_Position, 1.0);
    vec4 pos      = viewMatrix  * worldPos;

    fragNormal       = vec3(normalMatrix * vec4(in_Normal, 0.0));
    fragPos          = pos.xyz;
    texCoord         = in_TexCoord;
    fragPosLightSpace = lightSpaceMatrix * worldPos;

    gl_Position = projMatrix * pos;
}