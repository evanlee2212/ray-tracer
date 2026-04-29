#pragma once

#include "Mesh.h"
#include <string>

class TriMesh {
public:
  explicit TriMesh(const std::string& filepath);

  void draw() const;

  bool isValid() const { return m_valid; }

private:
  Mesh m_mesh;
  bool m_valid = false;

  static std::vector<float> loadFromFile(const std::string& filepath);
};