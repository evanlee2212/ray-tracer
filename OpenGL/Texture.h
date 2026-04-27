#pragma once

#include <GL/glew.h>
#include <string>

class Texture {
public:
  explicit Texture(const std::string& filepath);
  ~Texture();

  // Non-copyable, movable
  Texture(const Texture&) = delete;
  Texture& operator=(const Texture&) = delete;
  Texture(Texture&& other) noexcept;
  Texture& operator=(Texture&& other) noexcept;

  // Binds to GL_TEXTURE<unit>
  void bind(unsigned int unit = 0) const;
  void unbind() const;

  GLuint id()      const { return m_id; }
  bool   isValid() const { return m_id != 0; }

private:
  GLuint m_id = 0;

  void destroy();
};