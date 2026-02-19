//
// Created by Evan on 2/5/2026.
//

#ifndef CS4212STARTERC ODE_TRIANGLE_H
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
  Triangle() : vertex_a(point3(1,0,0)), vertex_b(point3(0, 1, 0)), vertex_c(point3(0, 0, 1)),
                shader(std::make_shared<LambertianShader>()), _color(0,0,0) {};

  Triangle(point3 a, point3 b, point3 c) : vertex_a(a), vertex_b(b), vertex_c(c),
            shader(std::make_shared<LambertianShader>()) {};

  Triangle(point3 a, point3 b, point3 c, std::shared_ptr<Shader> s, color color) : vertex_a(a), vertex_b(b), vertex_c(c),
            shader(s ? s : std::make_shared<LambertianShader>()), _color(color) {};

  bool intersect(const Ray &r, const float tmin, float &tmax, hitStructure &hitStruct) override;

};

#endif// CS4212STARTERCODE_TRIANGLE_H
