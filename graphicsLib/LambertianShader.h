//
// Created by Evan on 2/11/2026.
//

#ifndef CS4212STARTERCODE_LAMBERTIANSHADER_H
#define CS4212STARTERCODE_LAMBERTIANSHADER_H
#include "HitStructure.h"
#include "Shader.h"

class LambertianShader : public Shader {
private:
  color baseColor;

public:
  LambertianShader() : baseColor(255, 255, 255) {};
  LambertianShader(color baseColor) : baseColor(baseColor) {};

  color rayColor(const hitStructure &h)
  {
    color lightIntensity(255, 255, 255);
    point3 lightPos(0, 50, 0);

    float kd = 1.0f;
    vec3 dirToLight = unit_vector(lightPos - h.point);

    double ndot1 = std::max(0.0, dot(h.normal, dirToLight));

    color result = h.color * ndot1;

    return result;
  }
};

#endif// CS4212STARTERCODE_LAMBERTIANSHADER_H
