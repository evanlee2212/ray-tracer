//
// Created by Evan on 2/19/2026.
//
#include "LambertianShader.h"

#include "Scene.h"

color LambertianShader::rayColor(const hitStructure &h, int depth) {
  color materialColor = h.hitShape->getColor() / 255.0;
  color color(0.0, 0.0, 0.0);

  for (const auto& light : scene.getLights()) {
    vec3 lightDir = unit_vector(light->getPosition() - h.point);
    float diff = std::max(dot(h.normal, lightDir), 0.0);

    vec3 lightColor = light->getColor() / 255.0;

    color += diff * lightColor * light->getIntensity();
  }

  vec3 result = materialColor * color;

  result = vec3(
    std::min(result.x(), 1.0),
    std::min(result.y(), 1.0),
    std::min(result.z(), 1.0));

  return result * 255.0;
}