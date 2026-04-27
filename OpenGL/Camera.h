#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>

class Camera {
public:
  explicit Camera(glm::vec3 position       = glm::vec3(0, 2, 5),
                  float     moveSpeed      = 5.0f,
                  float     turnSpeedDeg   = 60.0f);

  void processInput(GLFWwindow* window, double deltaTime);

  glm::mat4 viewMatrix() const;

  glm::vec3 position() const { return m_pos; }
  glm::vec3 right()    const { return m_U;   }
  glm::vec3 up()       const { return m_V;   }
  glm::vec3 back()     const { return m_W;   }  // forward is -W

private:
  glm::vec3 m_pos;
  glm::vec3 m_U, m_V, m_W;   // right, up, backward

  float m_moveSpeed;          // world units per second
  float m_turnSpeed;          // radians per second

  // Per-frame helpers
  void yaw  (float angle);
  void pitch(float angle);
};