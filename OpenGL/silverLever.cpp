#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "GLSL.h"

int CheckGLErrors(const char *s)
{
    int errCount = 0;
    return errCount;
}

void generateSphere(glm::vec3 center, float radius, int slices, int stacks, std::vector<float>& vbo)
{
  for (int i = 0; i <= stacks; i++) {
    float theta = i * std::numbers::pi / stacks;

    for (int j = 0; j <= slices; j++) {
      float phi = j * 2.0f * std::numbers::pi / slices;

      // Vertices
      float x = radius * sin(theta) * cos(phi);
      float y = radius * cos(theta);
      float z = radius * sin(theta) * sin(phi);

      // Normals
      float nx = x / radius;
      float ny = y / radius;
      float nz = z / radius;

      vbo.push_back(center.x + x);
      vbo.push_back(center.y + y);
      vbo.push_back(center.z + z);
      vbo.push_back(nx);
      vbo.push_back(ny);
      vbo.push_back(nz);
    }
  }
}

void generateSphereIndices(int stacks, int slices, std::vector<unsigned int>& indices)
{
  for (int i = 0; i < stacks; i++) {
    for (int j = 0; j < slices; j++) {
      int row1 = i * (slices + 1);
      int row2 = (i + 1) * (slices + 1);

      // Triangle 1
      indices.push_back(row1 + j);
      indices.push_back(row1 + j + 1);
      indices.push_back(row2 + j + 1);

      // Triangle 2
      indices.push_back(row1 + j);
      indices.push_back(row2 + j + 1);
      indices.push_back(row2 + j);
    }
  }
}

GLuint loadSphere(std::vector<float> vbo, std::vector<unsigned int> indices)
{
  GLuint sphereVBO;
  glGenBuffers(1, &sphereVBO);
  glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
  glBufferData(GL_ARRAY_BUFFER, vbo.size() * sizeof(float), vbo.data(), GL_STATIC_DRAW);

  // EBO
  GLuint sphereEBO;
  glGenBuffers(1, &sphereEBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

  // VAO
  GLuint sphereVAO;
  glGenVertexArrays(1, &sphereVAO);
  glBindVertexArray(sphereVAO);
  glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
  glBindVertexArray(0);
  return sphereVAO;
}

std::vector<float> read_in_vertex_file(const std::string& filepath)
{
  // List of triangles
  std::ifstream file(filepath);
  if (!file.is_open()) {
    std::cerr << "Failed to open file" << std::endl;
  }

  std::vector<glm::vec3> positions;
  std::string tokenX, tokenY, tokenZ;

  while (getline(file, tokenX, ',') && getline(file, tokenY, ',') && getline(file, tokenZ, ',')) {
    try {
      float x = std::stof(tokenX);
      float y = std::stof(tokenY);
      float z = std::stof(tokenZ);

      positions.push_back(glm::vec3(x, y, z));

    } catch (const std::invalid_argument &e) {
      std::cerr << "Invalid token(s): " << tokenX << ' ' << tokenY << ' ' << tokenZ << std::endl;
    }
  }
  file.close();
  std::cout << "Vertices read: " << positions.size() / 6 << std::endl;

  // Calculate Normals for each vertex in list
  std::vector<glm::vec3> smoothNormals(positions.size(), glm::vec3(0.0f));
  for (int i = 0; i + 2 < positions.size(); i += 3) {
    glm::vec3 v0 = positions[i + 0];
    glm::vec3 v1 = positions[i + 1];
    glm::vec3 v2 = positions[i + 2];

    // Face normal
    glm::vec3 edge1 = v1 - v0;
    glm::vec3 edge2 = v2 - v0;
    glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

    smoothNormals[i + 0] += faceNormal;
    smoothNormals[i + 1] += faceNormal;
    smoothNormals[i + 2] += faceNormal;
  }

  // Build VBO
  std::vector<float> vbo_listOfTriangles;
  for (int i = 0; i < positions.size(); i++) {
    glm::vec3 v = positions[i];
    glm::vec3 n = glm::normalize(smoothNormals[i]);

    vbo_listOfTriangles.push_back(v.x);
    vbo_listOfTriangles.push_back(v.y);
    vbo_listOfTriangles.push_back(v.z);
    vbo_listOfTriangles.push_back(n.x);
    vbo_listOfTriangles.push_back(n.y);
    vbo_listOfTriangles.push_back(n.z);
  }
  return vbo_listOfTriangles;
}

int main(void)
{
  /* Initialize the library */
  if (!glfwInit()) {
    exit (-1);
  }
  // throw std::runtime_error("Error! initialization of glfw failed!");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  /* Create a windowed mode window and its OpenGL context */
  int winWidth = 1000;
  float aspectRatio = 1.0; // 16.0 / 9.0; // winWidth / (float)winHeight;
  int winHeight = winWidth / aspectRatio;

  GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "GLFW Example", NULL, NULL);
  if (!window) {
    std::cerr << "GLFW did not create a window!" << std::endl;

    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  GLenum err=glewInit();
  if(err != GLEW_OK) {
    std::cerr <<"GLEW Error! glewInit failed, exiting."<< std::endl;
    exit(EXIT_FAILURE);
  }

  const GLubyte* renderer = glGetString (GL_RENDERER);
  const GLubyte* version = glGetString (GL_VERSION);
  std::cout << "Renderer: " << renderer << std::endl;
  std::cout << "OpenGL version supported: " << version << std::endl;

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glClearColor(0.5, 0.7, .8, 1.0);

  int fb_width, fb_height;
  glfwGetFramebufferSize(window, &fb_width, &fb_height);
  glViewport(0, 0, fb_width, fb_height);

  // Need to set a projection matrix that fits the aspect ratio set
  // by the window frame.
  //
  // The ortho parameters, in order: left, right, bottom, top, zNear, zFar
  float halfWidth = 15.0 / 2.0;
  float halfHeight = halfWidth;

  float left = -halfWidth;
  float right = halfWidth;

  float bottom = -halfHeight;
  float top = halfHeight;

  float near = 5.0f;
  float far = -5.0f;

  //glm::mat4 M_ortho = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, near, far);
  glm::mat4 perspMat = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

  GLint major_version;
  glGetIntegerv(GL_MAJOR_VERSION, &major_version);
  std::cout << "GL_MAJOR_VERSION: " << major_version << std::endl;
   // ===================== SCENE SETUP =====================

  // Light that rotates around the scene
  glm::vec4 lightPosWorld(5.0f, 15.0f, 5.0f, 1.0f);
  float lightAngle = 0.0f;

  // Ground plane sphere
  std::vector<float> vboGround;
  std::vector<unsigned int> groundIndices;
  generateSphere(glm::vec3(0, -55, -15), 50.0f, 50, 50, vboGround);
  generateSphereIndices(50, 50, groundIndices);
  GLuint groundVAO = loadSphere(vboGround, groundIndices);
  int groundIndexCount = groundIndices.size();

  // Pedestal — flat wide sphere the bunny sits on
  std::vector<float> vboPedestal;
  std::vector<unsigned int> pedestalIndices;
  generateSphere(glm::vec3(0, -3.5f, -10), 2.0f, 30, 30, vboPedestal);
  generateSphereIndices(30, 30, pedestalIndices);
  GLuint pedestalVAO = loadSphere(vboPedestal, pedestalIndices);
  int pedestalIndexCount = pedestalIndices.size();

  struct FixedSphere {
    GLuint vao;
    int indexCount;
    glm::vec3 position;
    glm::vec3 color;
  };

  std::vector<FixedSphere> fixedSpheres;

  struct FixedSpec { float r; glm::vec3 pos; glm::vec3 col; };
  std::vector<FixedSpec> fixedSpecs = {
    { 0.6f, glm::vec3( 3.0f, -1.5f, -10.0f), glm::vec3(1.0f, 0.2f, 0.2f) }, // red right
    { 0.6f, glm::vec3(-3.0f, -1.5f, -10.0f), glm::vec3(0.2f, 0.4f, 1.0f) }, // blue left
    { 0.5f, glm::vec3( 0.0f, -1.5f, -13.0f), glm::vec3(0.2f, 1.0f, 0.4f) }, // green behind
    { 0.4f, glm::vec3( 2.0f,  0.5f, -12.0f), glm::vec3(1.0f, 0.8f, 0.1f) }, // gold upper right
    { 0.4f, glm::vec3(-2.0f,  0.5f, -12.0f), glm::vec3(1.0f, 0.3f, 0.8f) }, // pink upper left
};

  for (auto& s : fixedSpecs) {
    std::vector<float> vbo;
    std::vector<unsigned int> idx;
    generateSphere(s.pos, s.r, 20, 20, vbo);
    generateSphereIndices(20, 20, idx);
    fixedSpheres.push_back({ loadSphere(vbo, idx), (int)idx.size(), s.pos, s.col });
  }

  // Bunny VBO
  std::vector<float> vbo_listOfTriangles = read_in_vertex_file("../trilist.dat");
  GLuint bunnyVBO, bunnyVAO;
  glGenBuffers(1, &bunnyVBO);
  glBindBuffer(GL_ARRAY_BUFFER, bunnyVBO);
  glBufferData(GL_ARRAY_BUFFER, vbo_listOfTriangles.size() * sizeof(float), vbo_listOfTriangles.data(), GL_STATIC_DRAW);
  glGenVertexArrays(1, &bunnyVAO);
  glBindVertexArray(bunnyVAO);
  glBindBuffer(GL_ARRAY_BUFFER, bunnyVBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glBindVertexArray(0);

  // ===================== SHADERS =====================

  // Lambertian shader
  sivelab::GLSLObject shader_lamb;
  shader_lamb.addShader("vertexShader_PrepForPerFragment.glsl", sivelab::GLSLObject::VERTEX_SHADER);
  shader_lamb.addShader("fragmentShader_Lambertian.glsl", sivelab::GLSLObject::FRAGMENT_SHADER);
  shader_lamb.createProgram();

  GLuint l_proj   = shader_lamb.createUniform("projMatrix");
  GLuint l_view   = shader_lamb.createUniform("viewMatrix");
  GLuint l_model  = shader_lamb.createUniform("modelMatrix");
  GLuint l_normal = shader_lamb.createUniform("normalMatrix");
  GLuint l_light  = shader_lamb.createUniform("lightPosWorld");
  GLuint l_diff   = shader_lamb.createUniform("diffuseComponent");

  // Blinn-Phong shader
  sivelab::GLSLObject shader_bp;
  shader_bp.addShader("vertexShader_BlinnPhong.glsl", sivelab::GLSLObject::VERTEX_SHADER);
  shader_bp.addShader("fragmentShader_BlinnPhong.glsl", sivelab::GLSLObject::FRAGMENT_SHADER);
  shader_bp.createProgram();

  GLuint bp_proj     = shader_bp.createUniform("projMatrix");
  GLuint bp_view     = shader_bp.createUniform("viewMatrix");
  GLuint bp_model    = shader_bp.createUniform("modelMatrix");
  GLuint bp_normal   = shader_bp.createUniform("normalMatrix");
  GLuint bp_light    = shader_bp.createUniform("lightPosWorld");
  GLuint bp_intensity= shader_bp.createUniform("intensity");
  GLuint bp_la       = shader_bp.createUniform("la");
  GLuint bp_ka       = shader_bp.createUniform("ka");
  GLuint bp_kd       = shader_bp.createUniform("kd");
  GLuint bp_ks       = shader_bp.createUniform("ks");
  GLuint bp_phong    = shader_bp.createUniform("phongExp");

  glm::vec3 intensity(1.0f, 1.0f, 1.0f);
  glm::vec3 la(0.15f, 0.15f, 0.15f);
  glm::vec3 ka(0.15f, 0.15f, 0.15f);
  glm::vec3 ks(1.0f, 1.0f, 1.0f);
  float phongExp = 64.0f;

  shader_bp.activate();
  glUniform3fv(bp_intensity, 1, glm::value_ptr(intensity));
  glUniform3fv(bp_la,        1, glm::value_ptr(la));
  glUniform3fv(bp_ka,        1, glm::value_ptr(ka));
  glUniform3fv(bp_ks,        1, glm::value_ptr(ks));
  glUniform1f(bp_phong,      phongExp);
  shader_bp.deactivate();

  // Camera
  glm::vec3 m_pos(0, 2, 5);
  glm::vec3 m_U(1,0,0), m_V(0,1,0), m_W(0,0,1);

  // ===================== RENDER LOOP =====================
  auto drawLamb = [&](GLuint vao, int indexCount, glm::vec3 color, glm::mat4 model, glm::mat4 view, bool useElements) {
      glm::mat4 nm = glm::mat4(glm::transpose(glm::inverse(glm::mat3(view * model))));
      glUniformMatrix4fv(l_model,  1, GL_FALSE, glm::value_ptr(model));
      glUniformMatrix4fv(l_normal, 1, GL_FALSE, glm::value_ptr(nm));
      glUniform3fv(l_diff, 1, glm::value_ptr(color));
      glBindVertexArray(vao);
      if (useElements)
          glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
      else
          glDrawArrays(GL_TRIANGLES, 0, indexCount);
      glBindVertexArray(0);
  };

  auto drawBP = [&](GLuint vao, int indexCount, glm::vec3 kd, float exp, glm::mat4 model, glm::mat4 view, bool useElements) {
      glm::mat4 nm = glm::mat4(glm::transpose(glm::inverse(glm::mat3(view * model))));
      glUniformMatrix4fv(bp_model,  1, GL_FALSE, glm::value_ptr(model));
      glUniformMatrix4fv(bp_normal, 1, GL_FALSE, glm::value_ptr(nm));
      glUniform3fv(bp_kd,  1, glm::value_ptr(kd));
      glUniform1f(bp_phong, exp);
      glBindVertexArray(vao);
      if (useElements)
          glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
      else
          glDrawArrays(GL_TRIANGLES, 0, indexCount);
      glBindVertexArray(0);
  };

  double timeDiff = 0.0, startFrameTime = 0.0, endFrameTime = 0.0;

while (!glfwWindowShouldClose(window))
{
    // Animate light orbiting the scene
    //lightAngle += 0.005f;
    lightPosWorld = glm::vec4(
        12.0f * cos(lightAngle),
        10.0f,
        -10.0f + 12.0f * sin(lightAngle),
        1.0f
    );

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 M_view = glm::lookAt(m_pos, m_pos - m_W, m_V);

    // ---- Lambertian pass ----
    shader_lamb.activate();
    glUniformMatrix4fv(l_proj,  1, GL_FALSE, glm::value_ptr(perspMat));
    glUniformMatrix4fv(l_view,  1, GL_FALSE, glm::value_ptr(M_view));
    glUniform4fv(l_light, 1, glm::value_ptr(lightPosWorld));

    // Ground
    drawLamb(groundVAO, groundIndexCount, glm::vec3(0.35f, 0.55f, 0.25f),
             glm::mat4(1.0f), M_view, true);

    // Pedestal — white marble look
    drawLamb(pedestalVAO, pedestalIndexCount, glm::vec3(0.9f, 0.88f, 0.85f),
             glm::mat4(1.0f), M_view, true);

    // Surrounding spheres
    float t = glfwGetTime();
    for (auto& s: fixedSpheres) {
      drawLamb(s.vao, s.indexCount, s.color, glm::mat4(1.0f), M_view, true);
    }

    shader_lamb.deactivate();

    // ---- Blinn-Phong pass ----
    shader_bp.activate();
    glUniformMatrix4fv(bp_proj,  1, GL_FALSE, glm::value_ptr(perspMat));
    glUniformMatrix4fv(bp_view,  1, GL_FALSE, glm::value_ptr(M_view));
    glUniform4fv(bp_light, 1, glm::value_ptr(lightPosWorld));

    // Bunny
    glm::mat4 bunnyModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, -10.0f));
    bunnyModel = glm::scale(bunnyModel, glm::vec3(1.0f));  // adjust scale to fit your bunny
    drawBP(bunnyVAO, vbo_listOfTriangles.size() / 6,
           glm::vec3(0.95f, 0.90f, 0.80f), 128.0f, bunnyModel, M_view, false);

    shader_bp.deactivate();

    glfwSwapBuffers(window);
    glfwPollEvents();

    // Camera movement
    float moveRatePerFrame = 0.01f;
    float turnRatePerFrame = 0.001f;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
      m_pos = m_pos + -m_W * moveRatePerFrame;
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      m_pos = m_pos - m_U * moveRatePerFrame;
    else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
      m_pos = m_pos + m_W * moveRatePerFrame;
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      m_pos = m_pos + m_U * moveRatePerFrame;
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      m_pos = m_pos + m_V * moveRatePerFrame;
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      m_pos = m_pos - m_V * moveRatePerFrame;

    // Camera rotations
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
      glm::mat4 turn = glm::rotate(glm::mat4(1.0f), turnRatePerFrame, glm::vec3(0,1,0));
      m_U = glm::vec3(turn * glm::vec4(m_U, 0.0f));
      m_V = glm::vec3(turn * glm::vec4(m_V, 0.0f));
      m_W = glm::vec3(turn * glm::vec4(m_W, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
      glm::mat4 turn = glm::rotate(glm::mat4(1.0f), -turnRatePerFrame, glm::vec3(0,1,0));
      m_U = glm::vec3(turn * glm::vec4(m_U, 0.0f));
      m_V = glm::vec3(turn * glm::vec4(m_V, 0.0f));
      m_W = glm::vec3(turn * glm::vec4(m_W, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
      glm::mat4 turn = glm::rotate(glm::mat4(1.0f), turnRatePerFrame, m_U);
      m_V = glm::vec3(turn * glm::vec4(m_V, 0.0f));
      m_W = glm::vec3(turn * glm::vec4(m_W, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
      glm::mat4 turn = glm::rotate(glm::mat4(1.0f), -turnRatePerFrame, m_U);
      m_V = glm::vec3(turn * glm::vec4(m_V, 0.0f));
      m_W = glm::vec3(turn * glm::vec4(m_W, 0.0f));
    }


    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
      std::cout << "fps: " << 1.0 / timeDiff << std::endl;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, 1);
  }
  
    glfwTerminate();
    return 0;
}
