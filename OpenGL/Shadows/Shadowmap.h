#pragma once

#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../Objects/Light.h"

class ShadowMap {
public:
    static constexpr int DEFAULT_SIZE = 2048;

    ShadowMap(int width = DEFAULT_SIZE, int height = DEFAULT_SIZE);
    ~ShadowMap();

    // Non-copyable, movable
    ShadowMap(const ShadowMap&)            = delete;
    ShadowMap& operator=(const ShadowMap&) = delete;
    ShadowMap(ShadowMap&&)                 = default;

    // Binds the FBO and sets the viewport to shadow map resolution.
    // Call glClear(GL_DEPTH_BUFFER_BIT) and draw all shadow-casting geometry.
    void bindForWriting() const;

    // Binds the depth texture to the given texture unit for sampling.
    void bindForReading(unsigned int unit = 2) const;

    // ── Light matrices ────────────────────────────────────────────────────────
    void updateLight(const Light& light,
                     glm::vec3 sceneCenter = glm::vec3(0.0f),
                     float     nearPlane   = 1.0f,
                     float     farPlane    = 50.0f,
                     float     frustumSize = 20.0f);

    glm::mat4 lightSpaceMatrix() const { return m_lightProj * m_lightView; }
    glm::mat4 lightView()        const { return m_lightView; }
    glm::mat4 lightProj()        const { return m_lightProj; }

    int width()  const { return m_width;  }
    int height() const { return m_height; }
    GLuint depthTexture() const { return m_depthTex; }

private:
    int    m_width, m_height;
    GLuint m_fbo      = 0;
    GLuint m_depthTex = 0;

    glm::mat4 m_lightView = glm::mat4(1.0f);
    glm::mat4 m_lightProj = glm::mat4(1.0f);

    void init();
    void destroy();
};