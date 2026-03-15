//
// Created by Evan on 2/26/2026.
//

#ifndef CS4212STARTERCODE_POINTLIGHT_H
#define CS4212STARTERCODE_POINTLIGHT_H
#include "Light.h"
#include "vec3.h"

class PointLight : public Light {
public:
  PointLight(const vec3& pos, const vec3& color, float intensity=1.0f) :
    position(pos), color(color), intensity(intensity) {}

  vec3 getPosition() override;
  vec3 getColor() override;
  float getIntensity() override;

  std::string type = "Point Light";

private:
  vec3 position;
  vec3 color;
  float intensity = 1.0f;
};


#endif// CS4212STARTERCODE_POINTLIGHT_H
