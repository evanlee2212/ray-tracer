//
// Created by Evan on 2/4/2026.
//

#ifndef CS4212STARTERCODE_SHAPE_H
#define CS4212STARTERCODE_SHAPE_H
#include "HitStructure.h"
#include "ray.h"

class Shape
{
protected:
  color _color;
  std::shared_ptr<Shader> shader;

public:
  virtual ~Shape() = default;
  virtual bool intersect(const ray &r, const float tmin, float &tmax, hitStructure& hitStruct) = 0;
  Shader getShader();
};

#endif// CS4212STARTERCODE_SHAPE_H
