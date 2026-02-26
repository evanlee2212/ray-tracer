//
// Created by Evan on 2/4/2026.
//

#ifndef CS4212STARTERCODE_SPHERE_H
#define CS4212STARTERCODE_SPHERE_H
#include "Framebuffer.h"
#include "LambertianShader.h"
#include "Shape.h"
#include "vec3.h"
#include "Ray.h"
#include "Shader.h"

class Sphere : public Shape
{
  private:
    point3 center;
    float radius;
    std::shared_ptr<Shader> shader;
    color _color;

  public:
  Sphere(point3 center, float radius, std::shared_ptr<Shader> shader, color c) : center(center), radius(radius),
        shader(shader), _color(c) {};

  bool intersect(const Ray& r, const float tmin, float& tmax, hitStructure& hitStruct) override;

  std::shared_ptr<Shader> getShader();
  color getColor();
};

#endif// CS4212STARTERCODE_SPHERE_H
