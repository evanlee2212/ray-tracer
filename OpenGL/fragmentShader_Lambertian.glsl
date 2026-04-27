#version 330 core
layout(location=0) out vec4 fragmentColor;

uniform vec3 diffuseComponent;
uniform sampler2D textureUnit;

in vec3 normal;
in vec3 lightDir;

void main(void)
{
    vec3 N = normalize(normal);
    vec3 L = normalize(lightDir);

    float val = max(0.0, dot(N, L));
    vec3 diffuseShading = diffuseComponent * val;

    fragmentColor = vec4(diffuseShading, 1.0);
}