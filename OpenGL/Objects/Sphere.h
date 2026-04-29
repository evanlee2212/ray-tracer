#pragma once

#include "Mesh.h"
#include "glm/glm.hpp"
#include <vector>

class Sphere {
public:
  Sphere(glm::vec3 center, float radius, int slices, int stacks);

  void draw() const;

  glm::vec3 center() const { return m_center; }
  float     radius() const { return m_radius; }

private:
  glm::vec3 m_center;
  float     m_radius;
  Mesh      m_mesh;

  static void buildVertices(glm::vec3 center, float radius, int slices, int stacks,
                            std::vector<float>& vbo);
  static void buildIndices (int slices, int stacks,
                            std::vector<unsigned int>& indices);
};