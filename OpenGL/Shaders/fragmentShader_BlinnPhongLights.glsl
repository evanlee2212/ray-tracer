#version 410

#define MAX_LIGHTS 8

in vec3 fragNormal;
in vec3 fragPos;
in vec2 texCoord;

in vec3 lightDir[MAX_LIGHTS];
in vec3 halfVec[MAX_LIGHTS];

uniform int numLights;
uniform vec3 intensity[MAX_LIGHTS];
uniform vec3 la[MAX_LIGHTS];

uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float phongExp;

uniform sampler2D textureUnit;
uniform sampler2D specularTex;

out vec4 fragmentColor;

void main(void)
{
    vec3 N = normalize(fragNormal);

    vec3 diffuseColor   = texture(textureUnit, texCoord).rgb;
    float specularAmount = texture(specularTex,  texCoord).r;

    vec3 color = vec3(0.0);
    for (int i = 0; i < numLights; i++) {
        color += ka * la[i] * diffuseColor;
    }

    for (int i = 0; i < numLights; i++) {
        vec3 L = normalize(vec3(lightDir[i]));
        vec3 H = normalize(vec3(halfVec[i]));

        float diff = max(0.0, dot(N, L));
        float spec = pow(max(0.0, dot(N, H)), phongExp) * specularAmount;

        color += intensity[i] * (diff * diffuseColor);
        color += intensity[i] * (spec * ks);
    }

    fragmentColor = vec4(color, 1.0);

    if (numLights == 0) {
        fragmentColor = vec4(1.0, 0.0, 1.0, 1.0); // magenta = lights never set
        return;
    }
}