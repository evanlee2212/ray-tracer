//
// Created by Evan on 2/26/2026.
//

#ifndef CS4212STARTERCODE_LIGHT_H
#define CS4212STARTERCODE_LIGHT_H

#include "vec3.h"

class Light
{
public:
  virtual ~Light() = default;

  virtual vec3 getPosition() = 0;
  virtual vec3 getColor() = 0;
  virtual float getIntensity() = 0;

  std::string type;
};

#endif// CS4212STARTERCODE_LIGHT_H
