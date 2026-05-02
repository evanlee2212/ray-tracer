#version 410

#define MAX_LIGHTS 8

in vec3 fragNormal;
in vec3 fragPos;
in vec2 texCoord;
in vec4 fragPosLightSpace;

uniform mat4 viewMatrix;

uniform int  numLights;
uniform vec4 lightPosWorld[MAX_LIGHTS];
uniform vec3 intensity[MAX_LIGHTS];
uniform vec3 la[MAX_LIGHTS];

uniform vec3  ka;
uniform vec3  kd;
uniform vec3  ks;
uniform float phongExp;

uniform sampler2D       textureUnit;  // unit 0 — diffuse
uniform sampler2D       specularTex;  // unit 1 — specular
uniform sampler2DShadow shadowMap;    // unit 2 — depth (hardware PCF)

// ── Shadow ────────────────────────────────────────────────────────────────────
// PCF: samples a 3×3 neighbourhood around the shadow coord to soften edges.
// bias prevents shadow acne on surfaces facing away from the light.
float shadowFactor(vec4 fragPosLS, vec3 N, vec3 L)
{
    // Perspective divide → NDC [-1, 1], then remap to [0, 1] for texture lookup
    vec3 proj = fragPosLS.xyz / fragPosLS.w;
    proj      = proj * 0.5 + 0.5;

    // Fragment is outside the light frustum — treat as lit
    if (proj.z > 1.0) return 1.0;

    // Slope-scaled bias: steeper angles need more bias to avoid acne
    float bias   = max(0.005 * (1.0 - dot(N, L)), 0.001);
    proj.z      -= bias;

    // 3×3 PCF kernel — textureProj uses the sampler's built-in comparison
    float shadow    = 0.0;
    vec2  texelSize = vec2(1.0 / textureSize(shadowMap, 0));

    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            vec3 offset = vec3(vec2(x, y) * texelSize, 0.0);
            shadow += texture(shadowMap, proj + offset);
        }
    }
    return shadow / 9.0;  // 1.0 = fully lit, 0.0 = fully shadowed
}

// ── Main ──────────────────────────────────────────────────────────────────────
out vec4 fragmentColor;

void main()
{
    vec3  N             = normalize(fragNormal);
    vec3  viewDir       = normalize(-fragPos);
    vec3  diffuseColor  = texture(textureUnit, texCoord).rgb;
    float specularAmt   = texture(specularTex,  texCoord).r;

    vec3 color = vec3(0.0);
    for (int i = 0; i < numLights; i++)
    color += ka * la[i] * diffuseColor;

    // Diffuse + specular — light 0 is shadow-mapped, rest are unshadowed
    for (int i = 0; i < numLights; i++) {
        vec3 lp = vec3(viewMatrix * lightPosWorld[i]);
        vec3 L  = normalize(lp - fragPos);
        vec3 H  = normalize(L + viewDir);

        float diff = max(0.0, dot(N, L));
        float spec = pow(max(0.0, dot(N, H)), phongExp) * specularAmt;

        float shadow = (i == 0) ? shadowFactor(fragPosLightSpace, N, L) : 1.0;

        color += shadow * intensity[i] * (diff * diffuseColor);
        color += shadow * intensity[i] * (spec * ks);
    }

    fragmentColor = vec4(color, 1.0);
}