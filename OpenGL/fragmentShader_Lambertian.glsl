#version 330 core
layout(location=0) out vec4 fragmentColor;

uniform vec3 diffuseComponent;
uniform sampler2D textureUnit;

in vec3 normal;
in vec3 lightDir;
in vec2 tCoord;

void main(void)
{
    vec3 N = normalize(normal);
    vec3 L = normalize(lightDir);

    float val = max(0.0, dot(N, L));
    vec3 diffuseShading = diffuseComponent * val;

    vec3 kdTexel = texture(tectureUnit, tCoord).rgb;
    fragmentColor = vec4(diffuseShading, 1.0);
}