#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "GLSL.h"
#include "Objects/Camera.h"
#include "Objects/Light.h"
#include "Objects/Sphere.h"
#include "Objects/Trimesh.h"
#include "Objects/Texture.h"
#include "Shadows/ShadowMap.h"

struct ShaderLightUniforms {
    static constexpr int MAX_LIGHTS = 8;

    GLuint numLightsID              = 0;
    GLuint posIDs      [MAX_LIGHTS] = {};
    GLuint intensityIDs[MAX_LIGHTS] = {};
    GLuint laIDs       [MAX_LIGHTS] = {};

    void init(sivelab::GLSLObject& shader, bool hasAmbient = true)
    {
        numLightsID = shader.createUniform("numLights");
        for (int i = 0; i < MAX_LIGHTS; ++i) {
            const std::string idx = std::to_string(i);
            posIDs[i]       = shader.createUniform(("lightPosWorld[" + idx + "]").c_str());
            intensityIDs[i] = shader.createUniform(("intensity["     + idx + "]").c_str());
            if (hasAmbient)
                laIDs[i]    = shader.createUniform(("la["            + idx + "]").c_str());
        }
    }

    void upload(const std::vector<Light>& lights, bool hasAmbient = true) const
    {
        const int n = static_cast<int>(
            std::min(lights.size(), static_cast<size_t>(MAX_LIGHTS)));
        glUniform1i(numLightsID, n);
        for (int i = 0; i < n; ++i) {
            glUniform4fv(posIDs[i],       1, glm::value_ptr(lights[i].position));
            glUniform3fv(intensityIDs[i], 1, glm::value_ptr(lights[i].intensity));
            if (hasAmbient)
                glUniform3fv(laIDs[i],    1, glm::value_ptr(lights[i].ambient));
        }
    }
};

static glm::mat4 normalMatrix(const glm::mat4& view, const glm::mat4& model)
{
    return glm::mat4(glm::transpose(glm::inverse(glm::mat3(view * model))));
}

// ─────────────────────────────────────────────────────────────────────────────
int main()
{
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE,        GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    const int   WIN_WIDTH = 1000;
    const float ASPECT    = 1.0f;

    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, static_cast<int>(WIN_WIDTH / ASPECT),
                                          "OpenGL Rasterizer", nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW window creation failed\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) { std::cerr << "GLEW init failed\n"; return -1; }

    std::cout << "Renderer: "                << glGetString(GL_RENDERER) << '\n';
    std::cout << "OpenGL version supported: " << glGetString(GL_VERSION)  << '\n';

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.5f, 0.7f, 0.8f, 1.0f);

    int fb_w, fb_h;
    glfwGetFramebufferSize(window, &fb_w, &fb_h);

    const glm::mat4 perspMat = glm::perspective(glm::radians(45.0f), ASPECT, 0.1f, 100.0f);

    // ── Lights ────────────────────────────────────────────────────────────────
    std::vector<Light> lights = {
        Light(glm::vec3( 5.0f, 3.0f,  5.0f),   // key light — shadow mapped
              glm::vec3(1.0f, 0.95f, 0.85f),
              glm::vec3(0.10f, 0.10f, 0.12f)),

        Light(glm::vec3(-4.0f, 1.0f, -3.0f),   // fill — unshadowed
              glm::vec3(0.3f,  0.4f,  0.6f),
              glm::vec3(0.05f, 0.05f, 0.08f)),

        Light(glm::vec3( 0.0f, 8.0f,  0.0f),   // rim — unshadowed
              glm::vec3(0.5f,  0.5f,  0.5f),
              glm::vec3(0.0f)),
    };

    // Shadow map — tracks lights[0] only
    ShadowMap shadowMap(2048, 2048);

    // ── Scene objects ─────────────────────────────────────────────────────────
    Sphere  globe(glm::vec3(0, 0, -10), 2.0f, 40, 40);
    TriMesh bunny("../trilist.dat");
    Sphere ground(glm::vec3(0,-205, -10), 200.0f, 20, 20);

    Texture diffuseTex ("../textures/earth_daymap_2k.png");
    Texture specularTex("../textures/earth_specular_map_2k.png");

    // ── Camera ────────────────────────────────────────────────────────────────
    Camera camera(glm::vec3(0, 2, 5), 5.0f, 60.0f);

    // ── Depth pass shader ─────────────────────────────────────────────────────
    sivelab::GLSLObject shader_depth;
    shader_depth.addShader("Shaders/vertexShader_Depth.glsl",
                           sivelab::GLSLObject::VERTEX_SHADER);
    shader_depth.addShader("Shaders/fragmentShader_Depth.glsl",
                           sivelab::GLSLObject::FRAGMENT_SHADER);
    shader_depth.createProgram();

    const GLuint d_lightSpace = shader_depth.createUniform("lightSpaceMatrix");
    const GLuint d_model      = shader_depth.createUniform("modelMatrix");

    // ── Lambertian shader ─────────────────────────────────────────────────────
    sivelab::GLSLObject shader_lamb;
    shader_lamb.addShader("Shaders/vertexShader_LambertianLights.glsl",
                          sivelab::GLSLObject::VERTEX_SHADER);
    shader_lamb.addShader("Shaders/fragmentShader_LambertianLights.glsl",
                          sivelab::GLSLObject::FRAGMENT_SHADER);
    shader_lamb.createProgram();

    const GLuint l_proj       = shader_lamb.createUniform("projMatrix");
    const GLuint l_view       = shader_lamb.createUniform("viewMatrix");
    const GLuint l_model      = shader_lamb.createUniform("modelMatrix");
    const GLuint l_normal     = shader_lamb.createUniform("normalMatrix");
    const GLuint l_diff       = shader_lamb.createUniform("diffuseComponent");
    const GLuint l_lightSpace = shader_lamb.createUniform("lightSpaceMatrix");
    const GLuint l_shadowMap  = shader_lamb.createUniform("shadowMap");

    ShaderLightUniforms lambLights;
    lambLights.init(shader_lamb, false);

    shader_lamb.activate();
    glUniform1i(l_shadowMap, 2);
    shader_lamb.deactivate();

    // ── Blinn-Phong shader ────────────────────────────────────────────────────
    sivelab::GLSLObject shader_bp;
    shader_bp.addShader("Shaders/vertexShader_BlinnPhongLights.glsl",
                        sivelab::GLSLObject::VERTEX_SHADER);
    shader_bp.addShader("Shaders/fragmentShader_BlinnPhongLights.glsl",
                        sivelab::GLSLObject::FRAGMENT_SHADER);
    shader_bp.createProgram();

    const GLuint bp_proj       = shader_bp.createUniform("projMatrix");
    const GLuint bp_view       = shader_bp.createUniform("viewMatrix");
    const GLuint bp_model      = shader_bp.createUniform("modelMatrix");
    const GLuint bp_normal     = shader_bp.createUniform("normalMatrix");
    const GLuint bp_ka         = shader_bp.createUniform("ka");
    const GLuint bp_kd         = shader_bp.createUniform("kd");
    const GLuint bp_ks         = shader_bp.createUniform("ks");
    const GLuint bp_phong      = shader_bp.createUniform("phongExp");
    const GLuint bp_texUnit    = shader_bp.createUniform("textureUnit");
    const GLuint bp_specTex    = shader_bp.createUniform("specularTex");
    const GLuint bp_lightSpace = shader_bp.createUniform("lightSpaceMatrix");
    const GLuint bp_shadowMap  = shader_bp.createUniform("shadowMap");

    ShaderLightUniforms bpLights;
    bpLights.init(shader_bp, true);

    const glm::vec3 ka(0.15f), ks(1.0f);
    const float     phongExp = 32.0f;

    shader_bp.activate();
    glUniform3fv(bp_ka,     1, glm::value_ptr(ka));
    glUniform3fv(bp_ks,     1, glm::value_ptr(ks));
    glUniform1f (bp_phong,  phongExp);
    glUniform1i (bp_texUnit,  0);
    glUniform1i (bp_specTex,  1);
    glUniform1i (bp_shadowMap, 2);
    shader_bp.deactivate();

    // ── Draw helpers ──────────────────────────────────────────────────────────
    auto drawDepth = [&](const auto& mesh, const glm::mat4& model) {
        glUniformMatrix4fv(d_model, 1, GL_FALSE, glm::value_ptr(model));
        mesh.draw();
    };

    auto drawLamb = [&](const auto& mesh, const glm::vec3& color,
                        const glm::mat4& model, const glm::mat4& view)
    {
        glUniformMatrix4fv(l_model,  1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(l_normal, 1, GL_FALSE,
                           glm::value_ptr(normalMatrix(view, model)));
        glUniform3fv(l_diff, 1, glm::value_ptr(color));
        mesh.draw();
    };

    auto drawBP = [&](const auto& mesh, const glm::vec3& kd, float exp,
                      const glm::mat4& model, const glm::mat4& view)
    {
        glUniformMatrix4fv(bp_model,  1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(bp_normal, 1, GL_FALSE,
                           glm::value_ptr(normalMatrix(view, model)));
        glUniform3fv(bp_kd,    1, glm::value_ptr(kd));
        glUniform1f (bp_phong, exp);
        mesh.draw();
    };

    // ── Render loop ───────────────────────────────────────────────────────────
    double prevTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        const double now       = glfwGetTime();
        const double deltaTime = now - prevTime;
        prevTime = now;

        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
            std::cout << "fps: " << 1.0 / deltaTime << '\n';

        camera.processInput(window, deltaTime);

        // Animate key light
        const float t = static_cast<float>(now);
        lights[0].setPosition(glm::vec3(6.0f * std::cos(t * 0.5f),
                                        3.0f,
                                        6.0f * std::sin(t * 0.5f)));

        // Keep light-space matrix in sync with the moving light
        shadowMap.updateLight(lights[0], glm::vec3(0.0f), 1.0f, 50.0f, 20.0f);

        const glm::mat4 lightSpaceMat = shadowMap.lightSpaceMatrix();
        const glm::mat4 bunnyModel    = glm::mat4(1.0f);
        const glm::mat4 globeModel    = glm::mat4(1.0f);
        const glm::mat4 groundModel  =  glm::mat4(1.0f);

        // Depth pass
        shadowMap.bindForWriting();

        // Front-face culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

        shader_depth.activate();
        glUniformMatrix4fv(d_lightSpace, 1, GL_FALSE, glm::value_ptr(lightSpaceMat));
        drawDepth(bunny, bunnyModel);
        drawDepth(globe, globeModel);
        shader_depth.deactivate();

        glCullFace(GL_BACK);
        glDisable(GL_CULL_FACE);

        // Main shading pass
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, fb_w, fb_h);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const glm::mat4 M_view = camera.viewMatrix();

        // Bind shadow depth texture to unit 2 for both shaders
        shadowMap.bindForReading(2);

        // Lambertian
        shader_lamb.activate();
        glUniformMatrix4fv(l_proj,       1, GL_FALSE, glm::value_ptr(perspMat));
        glUniformMatrix4fv(l_view,       1, GL_FALSE, glm::value_ptr(M_view));
        glUniformMatrix4fv(l_lightSpace, 1, GL_FALSE, glm::value_ptr(lightSpaceMat));
        lambLights.upload(lights, false);
        drawLamb(bunny, glm::vec3(0.6f, 0.5f, 0.4f), bunnyModel, M_view);
        drawLamb(ground, glm::vec3(0.3f, 1.0f, 0.5f), groundModel, M_view);
        shader_lamb.deactivate();

        // Blinn-Phong
        shader_bp.activate();
        glUniformMatrix4fv(bp_proj,       1, GL_FALSE, glm::value_ptr(perspMat));
        glUniformMatrix4fv(bp_view,       1, GL_FALSE, glm::value_ptr(M_view));
        glUniformMatrix4fv(bp_lightSpace, 1, GL_FALSE, glm::value_ptr(lightSpaceMat));
        bpLights.upload(lights, true);
        diffuseTex .bind(0);
        specularTex.bind(1);
        // unit 2 already bound above
        drawBP(globe, glm::vec3(1.0f), phongExp, globeModel, M_view);
        shader_bp.deactivate();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
