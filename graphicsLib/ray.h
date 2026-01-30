//
// Created by Evan on 1/28/2026.
//

#ifndef CS4212STARTERCODE_RAY_H
#define CS4212STARTERCODE_RAY_H

#include "vec3.h"

class ray{
  private:
    point3 orig;
    vec3 dir;

  public:
    ray() : orig(0,0,0), dir(0,0,0) {};

  ray(const vec3& origin, const vec3& direction) : orig(origin), dir(direction) {}

  const point3 origin() const { return orig; }
  const vec3& direction() const { return dir; }

  point3 at(double t) const
    {
      return origin() + t*direction();
    }

};

#endif// CS4212STARTERCODE_RAY_H
