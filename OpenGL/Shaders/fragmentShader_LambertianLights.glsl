#version 410

#define MAX_LIGHTS 8

in vec3 fragNormal;
in vec3 fragPos;
in vec4 fragPosLightSpace;

uniform mat4 viewMatrix;

uniform int  numLights;
uniform vec4 lightPosWorld[MAX_LIGHTS];
uniform vec3 intensity[MAX_LIGHTS];
uniform vec3 diffuseComponent;

uniform sampler2DShadow shadowMap;  // unit 2

float shadowFactor(vec4 fragPosLS, vec3 N, vec3 L)
{
    vec3 proj = fragPosLS.xyz / fragPosLS.w;
    proj      = proj * 0.5 + 0.5;

    if (proj.z > 1.0) return 1.0;

    float bias  = max(0.005 * (1.0 - dot(N, L)), 0.001);
    proj.z     -= bias;

    float shadow    = 0.0;
    vec2  texelSize = vec2(1.0 / textureSize(shadowMap, 0));

    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            vec3 offset = vec3(vec2(x, y) * texelSize, 0.0);
            shadow += texture(shadowMap, proj + offset);
        }
    }
    return shadow / 9.0;
}

out vec4 fragmentColor;

void main()
{
    vec3 N = normalize(fragNormal);

    vec3 color = vec3(0.0);
    for (int i = 0; i < numLights; i++) {
        vec3 lp = vec3(viewMatrix * lightPosWorld[i]);
        vec3 L  = normalize(lp - fragPos);

        float diff   = max(0.0, dot(N, L));
        float shadow = (i == 0) ? shadowFactor(fragPosLightSpace, N, L) : 1.0;

        color += shadow * diffuseComponent * intensity[i] * diff;
    }

    fragmentColor = vec4(color, 1.0);
}