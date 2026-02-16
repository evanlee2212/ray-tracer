//
// Created by Evan on 2/11/2026.
//

#ifndef CS4212STARTERCODE_HITSTRUCTURE_H
#define CS4212STARTERCODE_HITSTRUCTURE_H

#include "vec3.h"
#include "Shader.h"
#include "ray.h"

#include <memory>

class Shape;

struct hitStructure
{
  float t = 0.0f;
  vec3 normal;
  point3 point;
  std::shared_ptr<Shape> hitShape = nullptr;
  std::shared_ptr<Shader> shader = nullptr;
  color color;
  ray r;
};

#endif// CS4212STARTERCODE_HITSTRUCTURE_H
