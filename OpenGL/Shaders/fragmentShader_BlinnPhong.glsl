#version 330 core

in vec3 normal;
in vec3 lightDir;
in vec3 viewDir;
in vec3 halfVec;
in vec2 texCoord;

layout(location=0) out vec4 fragmentColor;

uniform sampler2D textureUnit;
uniform sampler2D specularTex;
uniform vec3 intensity;
uniform vec3 la;
uniform vec3 ka;
uniform float phongExp;

void main(void)
{
    vec3 N = normalize(normal);
    vec3 L = normalize(lightDir);
    vec3 H = normalize(halfVec);

    vec3  diffuseColor   = texture(textureUnit, texCoord).rgb;
    float specularAmount = texture(specularTex,  texCoord).r;

    float diff = max(0.0, dot(N, L));
    float spec = pow(max(0.0, dot(N, H)), phongExp) * specularAmount;

    vec3 ambient  = ka * la * diffuseColor;
    vec3 diffuse  = intensity * diff * diffuseColor;
    vec3 specular = intensity * vec3(spec);

    fragmentColor = vec4(ambient + diffuse + specular, 1.0);
}