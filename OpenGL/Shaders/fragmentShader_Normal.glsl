#version 330 core

in vec4 normal;

layout(location=0) out vec4 fragmentColor;

void main(void)
{
    vec3 intensity = normalize(normal.xyz) * 0.5 + 0.5;
    fragmentColor = vec4(intensity, 1.0);
}