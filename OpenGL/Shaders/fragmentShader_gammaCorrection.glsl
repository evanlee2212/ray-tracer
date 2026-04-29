#version 410 core

in vec2 vTexCoords;

out vec4 FragColor;

uniform sampler2D fboTexture;
uniform float gamma;

void main()
{
    vec4 texelColor = texture(fboTexture, vTexCoords);

    // Apply gamma correction: C_out = C_in^(1/gamma)
    vec3 correctedColor = pow(texelColor.rgb, vec3(1.0 / gamma));

    //Posterization:
    //float levels = 8.0;
    //correctedColor = floor(texelColor.rgb * levels) / levels;

    FragColor = (correctedColor, texelColor.a);

    // ========== Sobel ========================================
    /**
    vec2 texelSize = 1.0 / textureSize(fboTexture, 0); // uv space is normalized

    vec3 c00 = texture(fboTexture, vTexCoords + texelSize * vec2(-1, -1)).rgb;
    vec3 c01 = texture(fboTexture, vTexCoords + texelSize * vec2( 0, -1)).rgb;
    vec3 c02 = texture(fboTexture, vTexCoords + texelSize * vec2( 1, -1)).rgb;

    vec3 c10 = texture(fboTexture, vTexCoords + texelSize * vec2(-1,  0)).rgb;
    vec3 c11 = texture(fboTexture, vTexCoords).rgb;
    vec3 c12 = texture(fboTexture, vTexCoords + texelSize * vec2( 1,  0)).rgb;

    vec3 c20 = texture(fboTexture, vTexCoords + texelSize * vec2(-1,  1)).rgb;
    vec3 c21 = texture(fboTexture, vTexCoords + texelSize * vec2( 0,  1)).rgb;
    vec3 c22 = texture(fboTexture, vTexCoords + texelSize * vec2( 1,  1)).rgb;

    // sobel
    vec3 gx =
    -1.0 * c00 +  1.0 * c02 +
    -2.0 * c10 +  2.0 * c12 +
    -1.0 * c20 +  1.0 * c22;

    vec3 gy =
    -1.0 * c00 + -2.0 * c01 + -1.0 * c02 +
    1.0 * c20 +  2.0 * c21 +  1.0 * c22;

    vec3 edge = sqrt(gx * gx + gy * gy);

    FragColor = vec4(edge, texelColor.a); **/

}