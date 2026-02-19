//
// Created by Evan on 2/4/2026.
//

#ifndef CS4212STARTERCODE_SPHERE_H
#define CS4212STARTERCODE_SPHERE_H
#include "Framebuffer.h"
#include "Shape.h"
#include "vec3.h"
#include "Ray.h"

class Sphere : public Shape
{
  private:
    point3 center;
    float radius;
    std::shared_ptr<Shader> shader;
    color _color;

  public:
  Sphere() : center(point3(0,0,0)), radius(1), shader(std::make_shared<LambertianShader>()) {};
  Sphere(point3 center, float radius) : center(center), radius(radius) {};
  Sphere(point3 center, float radius, std::shared_ptr<Shader> shader, color c) : center(center), radius(radius),
        shader(shader), _color(c) {};

  bool intersect(const Ray& r, const float tmin, float& tmax, hitStructure& hitStruct) override
  {
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

  std::shared_ptr<Shader> getShader() { return shader; }
};

#endif// CS4212STARTERCODE_SPHERE_H
