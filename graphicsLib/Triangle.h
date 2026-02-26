//
// Created by Evan on 2/5/2026.
//

#ifndef CS4212STARTERCODE_TRIANGLE_H
#define CS4212STARTERCODE_TRIANGLE_H
#include "Shape.h"
#include "Ray.h"
#include "vec3.h"
#include "Scene.h"
#include "LambertianShader.h"

class Triangle : public Shape
{
  private:
    point3 vertex_a, vertex_b, vertex_c;
    std::shared_ptr<Shader> shader;
    color _color;

  public:
  Triangle(point3 a, point3 b, point3 c, std::shared_ptr<Shader> s, color color) : vertex_a(a), vertex_b(b), vertex_c(c),
            shader(s), _color(color) {};

  bool intersect(const Ray &r, const float tmin, float &tmax, hitStructure &hitStruct) override;

  color getColor();
  std::shared_ptr<Shader> getShader();
};

#endif// CS4212STARTERCODE_TRIANGLE_H
