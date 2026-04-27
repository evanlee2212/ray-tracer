#include <cstdlib>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "GLSL.h"
#include "Camera.h"
#include "Sphere.h"
#include "TriMesh.h"
#include "Texture.h"

// Helpers


static glm::mat4 normalMatrix(const glm::mat4& view, const glm::mat4& model)
{
    return glm::mat4(glm::transpose(glm::inverse(glm::mat3(view * model))));
}

int main()
{
    // GLFW + window
    if (!glfwInit()) { return -1; }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    constexpr int   WIN_WIDTH = 1000;
    constexpr float ASPECT    = 1.0f;

    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_WIDTH / ASPECT,
                                          "OpenGL Rasterizer", nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW window creation failed\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW init failed\n";
        return -1;
    }

    std::cout << "Renderer: "               << glGetString(GL_RENDERER) << '\n';
    std::cout << "OpenGL version supported: "<< glGetString(GL_VERSION)  << '\n';

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.5f, 0.7f, 0.8f, 1.0f);

    int fb_w, fb_h;
    glfwGetFramebufferSize(window, &fb_w, &fb_h);
    glViewport(0, 0, fb_w, fb_h);

    // Projection
    const glm::mat4 perspMat = glm::perspective(glm::radians(45.0f),
                                                 ASPECT, 0.1f, 100.0f);

    // Scene objects
    const glm::vec4 lightPosWorld(5.0f, 3.0f, 5.0f, 1.0f);

    // Globe sphere (textured, Blinn-Phong)
    Sphere globe(glm::vec3(0, 0, -10), 2.0f, 40, 40);

    // Bunny mesh (Lambertian)
    TriMesh bunny("../trilist.dat");

    // Textures
    Texture diffuseTex ("../textures/earth_daymap_2k.png");
    Texture specularTex("../textures/earth_specular_map_2k.png");

    // Camera
    Camera camera(glm::vec3(0, 2, 5), 5.0f, 60.0f);

    // Shaders

    // Lambertian
    sivelab::GLSLObject shader_lamb;
    shader_lamb.addShader("vertexShader_PrepForPerFragment.glsl",
                          sivelab::GLSLObject::VERTEX_SHADER);
    shader_lamb.addShader("fragmentShader_Lambertian.glsl",
                          sivelab::GLSLObject::FRAGMENT_SHADER);
    shader_lamb.createProgram();

    const GLuint l_proj   = shader_lamb.createUniform("projMatrix");
    const GLuint l_view   = shader_lamb.createUniform("viewMatrix");
    const GLuint l_model  = shader_lamb.createUniform("modelMatrix");
    const GLuint l_normal = shader_lamb.createUniform("normalMatrix");
    const GLuint l_light  = shader_lamb.createUniform("lightPosWorld");
    const GLuint l_diff   = shader_lamb.createUniform("diffuseComponent");

    // Blinn-Phong (textured)
    sivelab::GLSLObject shader_bp;
    shader_bp.addShader("vertexShader_BlinnPhong.glsl",
                        sivelab::GLSLObject::VERTEX_SHADER);
    shader_bp.addShader("fragmentShader_BlinnPhong.glsl",
                        sivelab::GLSLObject::FRAGMENT_SHADER);
    shader_bp.createProgram();

    const GLuint bp_proj      = shader_bp.createUniform("projMatrix");
    const GLuint bp_view      = shader_bp.createUniform("viewMatrix");
    const GLuint bp_model     = shader_bp.createUniform("modelMatrix");
    const GLuint bp_normal    = shader_bp.createUniform("normalMatrix");
    const GLuint bp_light     = shader_bp.createUniform("lightPosWorld");
    const GLuint bp_intensity = shader_bp.createUniform("intensity");
    const GLuint bp_la        = shader_bp.createUniform("la");
    const GLuint bp_ka        = shader_bp.createUniform("ka");
    const GLuint bp_kd        = shader_bp.createUniform("kd");
    const GLuint bp_ks        = shader_bp.createUniform("ks");
    const GLuint bp_phong     = shader_bp.createUniform("phongExp");
    const GLuint bp_texUnit   = shader_bp.createUniform("textureUnit");
    const GLuint bp_specTex   = shader_bp.createUniform("specularTex");

    // Upload static uniforms once
    const glm::vec3 intensity(1.0f);
    const glm::vec3 la(0.15f), ka(0.15f), ks(1.0f);
    const float     phongExp = 32.0f;

    shader_bp.activate();
    glUniform3fv(bp_intensity, 1, glm::value_ptr(intensity));
    glUniform3fv(bp_la,        1, glm::value_ptr(la));
    glUniform3fv(bp_ka,        1, glm::value_ptr(ka));
    glUniform3fv(bp_ks,        1, glm::value_ptr(ks));
    glUniform1f (bp_phong,     phongExp);
    glUniform1i (bp_texUnit,   0);
    glUniform1i (bp_specTex,   1);
    shader_bp.deactivate();

    // Shader Functions
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
        glUniform3fv(bp_kd,   1, glm::value_ptr(kd));
        glUniform1f (bp_phong, exp);
        mesh.draw();
    };

    // Render loop
    double prevTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        const double now       = glfwGetTime();
        const double deltaTime = now - prevTime;
        prevTime = now;

        // FPS on T key
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
            std::cout << "fps: " << 1.0 / deltaTime << '\n';

        camera.processInput(window, deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const glm::mat4 M_view = camera.viewMatrix();

        // ── Lambertian pass ──────────────────────────────────────────────────
        shader_lamb.activate();
        glUniformMatrix4fv(l_proj,  1, GL_FALSE, glm::value_ptr(perspMat));
        glUniformMatrix4fv(l_view,  1, GL_FALSE, glm::value_ptr(M_view));
        glUniform4fv(l_light, 1, glm::value_ptr(lightPosWorld));

        //Lambertian Models:
        const glm::mat4 bunnyModel = glm::mat4(1.0f);
        drawLamb(bunny, glm::vec3(0.6f, 0.5f, 0.4f), bunnyModel, M_view);

        shader_lamb.deactivate();

        // Blinn-Phong pass
        shader_bp.activate();
        glUniformMatrix4fv(bp_proj,  1, GL_FALSE, glm::value_ptr(perspMat));
        glUniformMatrix4fv(bp_view,  1, GL_FALSE, glm::value_ptr(M_view));
        glUniform4fv(bp_light, 1, glm::value_ptr(lightPosWorld));

        diffuseTex .bind(0);
        specularTex.bind(1);

        //BP Models:
        const glm::mat4 globeModel = glm::mat4(1.0f);
        drawBP(globe, glm::vec3(1.0f), phongExp, globeModel, M_view);

        shader_bp.deactivate();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

