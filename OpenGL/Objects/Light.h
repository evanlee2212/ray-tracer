#pragma once

#include "glm/glm.hpp"

struct Light {
  glm::vec4 position;
  glm::vec3 intensity;
  glm::vec3 ambient;

  Light(glm::vec3 pos,
        glm::vec3 intensity = glm::vec3(1.0f),
        glm::vec3 ambient   = glm::vec3(0.15f))
      : position(pos, 1.0f)
      , intensity(intensity)
      , ambient(ambient)
  {}

  void setPosition(glm::vec3 pos) { position = glm::vec4(pos, 1.0f); }
  glm::vec3 pos3() const { return glm::vec3(position); }
};