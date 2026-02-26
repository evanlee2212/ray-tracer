//
// Created by Evan on 2/19/2026.
//
#include "BlinnPhongShader.h"

#include "Scene.h"

color BlinnPhongShader::rayColor(const hitStructure &h, int depth) {
  color materialColor = h.hitShape->getColor();
  vec3 color(0.0, 0.0, 0.0);

  float kd = 1.0f;
  float ks = 0.6f;
  float shininess = 32.0f;

  vec3 V = unit_vector(scene.getEyePosition() - h.point);

  for (const auto& light : scene.getLights()) {
    vec3 L = unit_vector(light->getPosition() - h.point);
    vec3 H = unit_vector(L + V);

    float diff = std::max(dot(h.normal, L), 0.0);
    vec3 diffuse = kd * diff * light->getColor() * light->getIntensity();

    float spec = std::pow(std::max(dot(h.normal, H), 0.0), shininess);
    vec3 specular = ks * spec * light->getColor() * light->getIntensity();

    color += diffuse * materialColor + specular;
  }

  color = vec3(
    std::min(color.x(), 1.0),
    std::min(color.y(), 1.0),
    std::min(color.z(), 1.0)
  );

  return color;
}