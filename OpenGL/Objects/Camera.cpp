#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(glm::vec3 position, float moveSpeed, float turnSpeedDeg)
    : m_pos(position)
    , m_U(1, 0, 0)
    , m_V(0, 1, 0)
    , m_W(0, 0, 1)
    , m_moveSpeed(moveSpeed)
    , m_turnSpeed(glm::radians(turnSpeedDeg))
{}

glm::mat4 Camera::viewMatrix() const
{
    return glm::lookAt(m_pos, m_pos - m_W, m_V);
}

void Camera::processInput(GLFWwindow* window, double deltaTime)
{
    const float dt    = static_cast<float>(deltaTime);
    const float move  = m_moveSpeed * dt;
    const float turn  = m_turnSpeed * dt;

    // Translation — only first pressed key wins to avoid diagonal conflicts
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        m_pos -= m_W * move;
    else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        m_pos += m_W * move;
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_pos -= m_U * move;
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_pos += m_U * move;
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_pos += m_V * move;
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_pos -= m_V * move;

    // Rotation — multiple rotation keys can fire simultaneously
    if (glfwGetKey(window, GLFW_KEY_LEFT)  == GLFW_PRESS) yaw( turn);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) yaw(-turn);
    if (glfwGetKey(window, GLFW_KEY_UP)    == GLFW_PRESS) pitch( turn);
    if (glfwGetKey(window, GLFW_KEY_DOWN)  == GLFW_PRESS) pitch(-turn);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}

void Camera::yaw(float angle)
{
    const glm::mat4 R = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0));
    m_U = glm::vec3(R * glm::vec4(m_U, 0.0f));
    m_V = glm::vec3(R * glm::vec4(m_V, 0.0f));
    m_W = glm::vec3(R * glm::vec4(m_W, 0.0f));
}

void Camera::pitch(float angle)
{
    const glm::mat4 R = glm::rotate(glm::mat4(1.0f), angle, m_U);
    m_V = glm::vec3(R * glm::vec4(m_V, 0.0f));
    m_W = glm::vec3(R * glm::vec4(m_W, 0.0f));
}