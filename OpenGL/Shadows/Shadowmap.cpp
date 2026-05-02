#include "Shadowmap.h"
#include <iostream>

ShadowMap::ShadowMap(int width, int height)
    : m_width(width), m_height(height)
{
    init();
}

ShadowMap::~ShadowMap() { destroy(); }

// ─── GPU resource setup ───────────────────────────────────────────────────────
void ShadowMap::init()
{
    // Depth texture
    glGenTextures(1, &m_depthTex);
    glBindTexture(GL_TEXTURE_2D, m_depthTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
                 m_width, m_height, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    // Clamp to border so regions outside the light frustum are NOT shadowed
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    glBindTexture(GL_TEXTURE_2D, 0);

    // FBO — depth only, no colour attachment
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D, m_depthTex, 0);

    // disable color read/write
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "[ShadowMap] FBO incomplete: 0x"
                  << std::hex << status << '\n';

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::destroy()
{
    if (m_fbo)      { glDeleteFramebuffers(1, &m_fbo);      m_fbo      = 0; }
    if (m_depthTex) { glDeleteTextures(1,     &m_depthTex); m_depthTex = 0; }
}

//  write depth
void ShadowMap::bindForWriting() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_width, m_height);
    glClear(GL_DEPTH_BUFFER_BIT);
}

// read depth
void ShadowMap::bindForReading(unsigned int unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_depthTex);
}

//  Light matrix update
void ShadowMap::updateLight(const Light& light,
                            glm::vec3 sceneCenter,
                            float nearPlane,
                            float farPlane,
                            float frustumSize)
{
    glm::vec3 lightPos = light.pos3();

    // View from light looking at scene center
    m_lightView = glm::lookAt(lightPos, sceneCenter, glm::vec3(0, 1, 0));

    // Orthographic projection covers the scene from the light's POV.
    // Increase frustumSize if shadows get clipped at scene edges.
    const float h = frustumSize * 0.5f;
    m_lightProj = glm::ortho(-h, h, -h, h, nearPlane, farPlane);
}