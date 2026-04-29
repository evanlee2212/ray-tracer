#version 330 core
layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec2 in_TexCoord;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;
uniform vec4 lightPosWorld;

out vec3 normal;
out vec3 lightDir;
out vec2 tCoord;

void main(void)
{
    vec4 pos = viewMatrix * modelMatrix * vec4(in_Position, 1.0);
    vec4 lightPos = viewMatrix * lightPosWorld;

    normal = vec3(normalMatrix * vec4(in_Normal, 0.0));
    lightDir = normalize(lightPos.xyz - pos.xyz);

    gl_Position = projMatrix * pos;

    tCoord = in_TexCoord;
}