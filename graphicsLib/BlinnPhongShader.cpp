//
// Created by Evan on 2/19/2026.
//
#include "BlinnPhongShader.h"

#include "Scene.h"

color BlinnPhongShader::rayColor(const hitStructure &h, int depth) {
  color materialColor = h.hitShape->getColor() / 255.0;
  vec3 finalColor(0.0, 0.0, 0.0);

  //Small base illumination
  float ambient = 0.1;
  finalColor += ambient * materialColor;

  float kd = 1.0f;
  float ks = 0.6f;
  float shininess = 32.0f;

  vec3 V = unit_vector(scene.getEyePosition() - h.point);

  for (const auto& light : scene.getLights()) {
    vec3 L = unit_vector(light->getPosition() - h.point);
    vec3 H = unit_vector(L + V);

    bool inShadow = false;
    Ray shadowRay = Ray(h.point, L);
    float distanceToLight = (light->getPosition() - h.point).length();

    for (const auto& shape : scene.getShapes()) {
      hitStructure shadowHit;
      if (shape->intersect(shadowRay, 0.001f, distanceToLight, shadowHit)) {
        inShadow = true;
        break;
      }
    }

    if (!inShadow) {
      vec3 lightColor = light->getColor() / 255.0;
      float intensity = light->getIntensity();

      float diff = std::max(dot(h.normal, L), 0.0);

      if (diff > 0.0f) {
        vec3 diffuse = kd * diff * lightColor * intensity * materialColor;

        float spec = std::pow(std::max(dot(h.normal, H), 0.0), shininess);
        vec3 specular = ks * spec * lightColor * intensity;

        finalColor += diffuse + specular;
      }
    }
  }

  vec3 result = vec3(
    std::min(finalColor.x(), 1.0),
    std::min(finalColor.y(), 1.0),
    std::min(finalColor.z(), 1.0)
  );

  return result;
}