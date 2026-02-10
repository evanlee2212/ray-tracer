//
// Created by Evan on 2/4/2026.
//

#ifndef CS4212STARTERCODE_SPHERE_H
#define CS4212STARTERCODE_SPHERE_H
#include "Framebuffer.h"
#include "Shape.h"
#include "vec3.h"
#include "ray.h"

class Sphere : public Shape
{
  private:
    point3 center;
    float radius;

  public:
  Sphere() : center(point3(0,0,0)), radius(1) {};
  Sphere(point3 center, float radius) : center(center), radius(radius) {};

  bool intersect(const ray& r, const float tmin, float& tmax) override
  {
    vec3 oc = r.origin() - center;

    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;

    float discriminant = b*b - 4*a*c;

    if (discriminant < 0.0f)
      return false;

    float sqrt_d = std::sqrt(discriminant);

    float t = (-b - sqrt_d) / 2*a;
    if (t < tmin) {
      return false;
    }

    tmax = t;
    return true;
  }

};

#endif// CS4212STARTERCODE_SPHERE_H
