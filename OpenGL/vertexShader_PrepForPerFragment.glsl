#version 330 core

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;

out vec4 normal;
out vec3 halfVec;
out vec3 lightDir;

uniform vec4 lightPosWorld;
uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

void main(void) {
    // Transform position to view space
    vec4 pos = viewMatrix * modelMatrix * vec4(in_Position, 1.0);
    vec4 lightPos = viewMatrix * lightPosWorld;

    // Transform normal to view space
    normal = normalMatrix * vec4(in_Normal, 0.0);

    // Compute view, light, and half vectors in view space
    vec3 v = normalize(-pos.xyz);
    lightDir = normalize(lightPos.xyz - pos.xyz);
    halfVec = normalize(v + lightDir);

    gl_Position = projMatrix * pos;
}