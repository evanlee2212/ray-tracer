//
// Created by Evan on 2/19/2026.
//
#include "Sphere.h"

#include "Scene.h"

bool Sphere::intersect(const Ray &r, const float tmin, float &tmax, hitStructure &hitStruct){
  vec3 oc = r.origin() - center;

  float a = dot(r.direction(), r.direction());
  float b = 2.0f * dot(oc, r.direction());
  float c = dot(oc, oc) - (radius * radius);

  float discriminant = b*b - 4*a*c;

  if (discriminant < 0.0f)
    return false;

  float sqrt_d = std::sqrt(discriminant);

  float t = (-b - sqrt_d) / (2*a);
  if (t < tmin) {
    return false;
  }

  tmax = t;

  //Update Hit structure
  hitStruct.t = t;
  hitStruct.point = r.at(tmax);
  hitStruct.normal = unit_vector(hitStruct.point - center);
  hitStruct.shader = getShader();
  hitStruct.color = _color;

  return true;
}

std::shared_ptr<Shader> Sphere::getShader() { return shader; }
color Sphere::getColor() { return _color; }