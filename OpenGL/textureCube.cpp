#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "../src/png++/png.hpp"

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

      //Texture Coords
      float u = (float)i / slices;
      float v = (float)j / stacks;

      vbo.push_back(center.x + x);
      vbo.push_back(center.y + y);
      vbo.push_back(center.z + z);
      vbo.push_back(nx);
      vbo.push_back(ny);
      vbo.push_back(nz);
      vbo.push_back(u);
      vbo.push_back(v);
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

  //Position
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);

  //Normal
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));

  //Texture Coords
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2,2,GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));

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
  // Initialize the library
  if (!glfwInit()) {
    exit (-1);
  }
  // throw std::runtime_error("Error! initialization of glfw failed!");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Create a windowed mode window and its OpenGL context
  int winWidth = 1000;
  float aspectRatio = 1.0; // 16.0 / 9.0; // winWidth / (float)winHeight;
  int winHeight = winWidth / aspectRatio;

  GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "GLFW Example", NULL, NULL);
  if (!window) {
    std::cerr << "GLFW did not create a window!" << std::endl;

    glfwTerminate();
    return -1;
  }

  // Make the window's context current
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

  // Light
  glm::vec4 lightPosWorld(5.0f, 3.0f, 5.0f, 1.0f);

  //Loading Texture
  std::string texFilename = "../textures/textureAtlas.png";
  std::cout << "Reading texture map data from file: " << texFilename << std::endl;
  png::image<png::rgb_pixel> texPNGImage;
  texPNGImage.read(texFilename);

  int pngWidth = texPNGImage.get_width();
  int pngHeight = texPNGImage.get_height();

  std::vector<float> texData(pngHeight * pngWidth * 3);

  size_t idx = 0;
  for (size_t row = 0; row < pngHeight; ++row) {
    for (size_t col = 0; col < pngWidth; ++col) {
      png::rgb_pixel pixel = texPNGImage[pngHeight - row - 1][col]; // flip of height!!!
      texData[idx++] = pixel.red / 255.0f;
      texData[idx++] = pixel.green / 255.0f;
      texData[idx++] = pixel.blue / 255.0f;
    }
  }

    //Transfer texture to GPU
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 pngWidth, pngHeight,
                 0, GL_RGB, GL_FLOAT, texData.data());
    glBindTexture(GL_TEXTURE_2D, 0);

    // Box Data
    /** struct vertexData {
      glm::vec3 pos;
      glm::vec3 normal;
      glm::vec2 texCoord;
    }; **/

    std::vector<float> boxVBO = {
      //Position (3) + Normal (3) + TexCoord (2)

      //bottom-left quadrant of texture
      -1.0f, -1.0f,  1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
       1.0f, -1.0f,  1.0f,   0.0f, 0.0f, 1.0f,   0.5f, 0.0f,
       1.0f,  1.0f,  1.0f,   0.0f, 0.0f, 1.0f,   0.5f, 0.5f,

       1.0f,  1.0f,  1.0f,   0.0f, 0.0f, 1.0f,   0.5f, 0.5f,
      -1.0f,  1.0f,  1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.5f,
      -1.0f, -1.0f,  1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,

      //bottom-right quadrant of texture
       1.0f, -1.0f,  1.0f,   1.0f, 0.0f, 0.0f,   0.5f, 0.0f,
       1.0f, -1.0f, -1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
       1.0f,  1.0f, -1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.5f,

       1.0f,  1.0f, -1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.5f,
       1.0f,  1.0f,  1.0f,   1.0f, 0.0f, 0.0f,   0.5f, 0.5f,
       1.0f, -1.0f,  1.0f,   1.0f, 0.0f, 0.0f,   0.5f, 0.0f,

      // top-left quadrant of texture
      -1.0f,  1.0f,  1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.5f,
       1.0f,  1.0f,  1.0f,   0.0f, 1.0f, 0.0f,   0.5f, 0.5f,
       1.0f,  1.0f, -1.0f,   0.0f, 1.0f, 0.0f,   0.5f, 1.0f,

       1.0f,  1.0f, -1.0f,   0.0f, 1.0f, 0.0f,   0.5f, 1.0f,
      -1.0f,  1.0f, -1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
      -1.0f,  1.0f,  1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.5f,

      // top-right quadrant of texture
       1.0f, -1.0f, -1.0f,   0.0f, 0.0f, -1.0f,  0.5f, 0.5f,
      -1.0f, -1.0f, -1.0f,   0.0f, 0.0f, -1.0f,  1.0f, 0.5f,
      -1.0f,  1.0f, -1.0f,   0.0f, 0.0f, -1.0f,  1.0f, 1.0f,

      -1.0f,  1.0f, -1.0f,   0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
       1.0f,  1.0f, -1.0f,   0.0f, 0.0f, -1.0f,  0.5f, 1.0f,
       1.0f, -1.0f, -1.0f,   0.0f, 0.0f, -1.0f,  0.5f, 0.5f,
  };

    // VBO and VAO
    GLuint boxVBOID, boxVAO;
    glGenBuffers(1, &boxVBOID);
    glBindBuffer(GL_ARRAY_BUFFER, boxVBOID);
    glBufferData(GL_ARRAY_BUFFER, boxVBO.size() * sizeof(float), boxVBO.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &boxVAO);
    glBindVertexArray(boxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, boxVBOID);

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

    // Texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    glUniform1i(texID, 0);
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
    GLuint bp_texUnit  = shader_bp.createUniform("textureUnit");

    glm::vec3 intensity(1.0f, 1.0f, 1.0f);
    glm::vec3 la(0.15f, 0.15f, 0.15f);
    glm::vec3 ka(0.15f, 0.15f, 0.15f);
    glm::vec3 ks(1.0f, 1.0f, 1.0f);
    float phongExp = 32.0f;

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

    // Shader functions
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

    // ===================== RENDER LOOP =====================
    while (!glfwWindowShouldClose(window))
    {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glm::mat4 M_view = glm::lookAt(m_pos, m_pos - m_W, m_V);

      // ---- Lambertian pass ----
      shader_lamb.activate();
      glUniformMatrix4fv(l_proj,  1, GL_FALSE, glm::value_ptr(perspMat));
      glUniformMatrix4fv(l_view,  1, GL_FALSE, glm::value_ptr(M_view));
      glUniform4fv(l_light, 1, glm::value_ptr(lightPosWorld));


      float t = glfwGetTime();


      shader_lamb.deactivate();

      // ---- Blinn-Phong pass ----
      shader_bp.activate();
      glUniformMatrix4fv(bp_proj,  1, GL_FALSE, glm::value_ptr(perspMat));
      glUniformMatrix4fv(bp_view,  1, GL_FALSE, glm::value_ptr(M_view));
      glUniform4fv(bp_light, 1, glm::value_ptr(lightPosWorld));

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texID);
      glUniform1i(bp_texUnit, 0);

      glm::mat4 boxModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
      //boxModel = glm::scale(boxModel, glm::vec3(3.0f));
      glm::mat4 boxNormal = glm::mat4(glm::transpose(glm::inverse(glm::mat3(M_view * boxModel))));
      glUniformMatrix4fv(bp_model, 1, GL_FALSE, glm::value_ptr(boxModel));\
      glUniformMatrix4fv(bp_normal, 1, GL_FALSE, glm::value_ptr(boxNormal));

      glBindVertexArray(boxVAO);
      glDrawArrays(GL_TRIANGLES, 0, boxVBO.size() / 8);
      glBindVertexArray(0);

      shader_bp.deactivate();

      glfwSwapBuffers(window);
      glfwPollEvents();

      // Camera movement
      float moveRatePerFrame = 0.001f;
      float turnRatePerFrame = 0.0001f;
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
};
