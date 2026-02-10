//
// Created by Evan on 2/4/2026.
//

#ifndef CS4212STARTERCODE_SHAPE_H
#define CS4212STARTERCODE_SHAPE_H
#include "ray.h"

class Shape
{
  public:
  virtual ~Shape() = default;
  virtual bool intersect(const ray &r, const float tmin, float &tmax) = 0;
};

#endif// CS4212STARTERCODE_SHAPE_H
