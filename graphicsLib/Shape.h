//
// Created by Evan on 2/4/2026.
//

#ifndef CS4212STARTERCODE_SHAPE_H
#define CS4212STARTERCODE_SHAPE_H
#include "ray.h"

class Shape
{
  public:
  virtual bool intersect(const ray &r) = 0;
};

#endif// CS4212STARTERCODE_SHAPE_H
