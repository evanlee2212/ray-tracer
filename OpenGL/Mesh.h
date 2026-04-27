#pragma once

#include <vector>
#include <GL/glew.h>

class Mesh {
public:
  Mesh(const std::vector<float>& vertices,
       const std::vector<unsigned int>& indices,
       bool hasTexCoords = true);

  explicit Mesh(const std::vector<float>& vertices,
                bool hasTexCoords = false);

  ~Mesh();

  Mesh(const Mesh&) = delete;
  Mesh& operator=(const Mesh&) = delete;
  Mesh(Mesh&& other) noexcept;
  Mesh& operator=(Mesh&& other) noexcept;

  void draw() const;

  int  vertexCount()  const { return m_vertexCount;  }
  int  indexCount()   const { return m_indexCount;   }
  bool isIndexed()    const { return m_indexed;       }

private:
  GLuint m_vao = 0;
  GLuint m_vbo = 0;
  GLuint m_ebo = 0;

  int  m_vertexCount = 0;
  int  m_indexCount  = 0;
  bool m_indexed     = false;

  void upload(const std::vector<float>& vertices,
              const std::vector<unsigned int>* indices,
              bool hasTexCoords);

  void destroy();
};