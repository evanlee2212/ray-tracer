//
// Created by Evan on 2/4/2026.
//

#ifndef CS4212STARTERCODE_SPHERE_H
#define CS4212STARTERCODE_SPHERE_H
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

  bool intersect(const ray &r)
  {
    vec3 oc = r.origin() - center;

    float A = dot(r.direction(), r.direction());
    float B = 2.0f * dot(r.direction(), oc);
    float C = dot(oc, oc) - radius * radius;

    float discriminant = B*B - 4*A*C;

    return discriminant >= 0;
  }
};

#endif// CS4212STARTERCODE_SPHERE_H
