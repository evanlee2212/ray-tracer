//
// Created by Evan on 2/19/2026.
//
#include "BlinnPhongShader.h"

#include "Scene.h"

color BlinnPhongShader::rayColor(const hitStructure &h, int depth) {
  color lightIntensity(255, 255, 255);
  point3 lightPos(0, -50, 0);

  float kd = 0.8f;
  float ks = 0.5f;
  float shininess = 32.0f;

  vec3 dirToLight = unit_vector(lightPos - h.point);
  double ndot1 = std::max(0.0, dot(h.normal, dirToLight));
  color diffuse = h.color * kd * ndot1;

  vec3 viewDir = unit_vector(-h.r.direction());
  vec3 halfVec = unit_vector(dirToLight + viewDir);
  float ndoth = std::max(0.0, dot(halfVec, h.normal));
  float specular = ks * std::pow(ndoth, shininess);

  color result = diffuse + lightIntensity * specular;

  return result;
}