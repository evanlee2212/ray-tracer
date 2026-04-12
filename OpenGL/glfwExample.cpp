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

  //Initialize Data and get to GPU
  //Load Scene file
  //Get Shapes into scene

  // List of triangles
  std::ifstream file("../trilist.dat");
  if (!file.is_open()) {
    std::cerr << "Failed to open file" << std::endl;
  }

  std::vector<glm::vec3> positions;
  std::string tokenX, tokenY, tokenZ;

  while(getline(file, tokenX, ',') &&
        getline(file, tokenY, ',') &&
        getline(file, tokenZ, ',')) {
    try {
      float x = std::stof(tokenX);
      float y = std::stof(tokenY);
      float z = std::stof(tokenZ);

      positions.push_back(glm::vec3(x,y,z));

    } catch (const std::invalid_argument& e) {
      std::cerr << "Invalid token(s): " << tokenX << ' ' << tokenY << ' ' << tokenZ << std::endl;
    }
  }
  file.close();
  std::cout << "Vertices read: " << positions.size() / 6 << std::endl;

  // Calculate Normals for each vertex in list
  std::vector<glm::vec3> smoothNormals(positions.size(), glm::vec3(0.0f));
  for (int i = 0; i + 2 < positions.size(); i += 3) {
    glm::vec3 v0 = positions[i+0];
    glm::vec3 v1 = positions[i+1];
    glm::vec3 v2 = positions[i+2];

    // Face normal
    glm::vec3 edge1 = v1 - v0;
    glm::vec3 edge2 = v2 - v0;
    glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

    smoothNormals[i+0] += faceNormal;
    smoothNormals[i+1] += faceNormal;
    smoothNormals[i+2] += faceNormal;
  }

  //Build VBO
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

  GLuint triVBO, triVAO;

  glGenBuffers(1, &triVBO);
  glBindBuffer(GL_ARRAY_BUFFER, triVBO);
  glBufferData(GL_ARRAY_BUFFER, vbo_listOfTriangles.size() * sizeof(float), vbo_listOfTriangles.data(), GL_STATIC_DRAW);

  glGenVertexArrays(1, &triVAO);
  glBindVertexArray(triVAO);
  glBindBuffer(GL_ARRAY_BUFFER, triVBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glBindVertexArray(0);

  // Sphere
  std::vector<float> vboSphere;
  std::vector<unsigned int> indices;

  generateSphere(glm::vec3(0,0,0), 1.0f, 20, 20, vboSphere);
  generateSphereIndices(20, 20, indices);
  GLuint sphereVAO = loadSphere(vboSphere, indices);

  //Triangle
  GLuint m_triangleVBO[1], m_VAO;

  glGenBuffers(1, m_triangleVBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO[0]);

  std::vector<float> host_VertexBuffer{
    // Position           //Normal
    -3.0f, 3.0f, 0.0f,   0.0f, 0.0f, 1.0f,
    3.0f, 3.5f, 0.0f,   0.0f, 0.0f, 1.0f,
    0.0f, 6.0f, 0.0f,    0.0f, 0.0f, 1.0f,};

  int numBytes = host_VertexBuffer.size() * sizeof(float);

  glBufferData(GL_ARRAY_BUFFER, numBytes, host_VertexBuffer.data(), GL_STATIC_DRAW);
  host_VertexBuffer.clear();

  //VAO for VBO
  // create a vertex array object that will map the attributes in
  // our vertex buffer to different location attributes for our
  // shaders
  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  // VAO details here - we only have 1 attribute or location
  // (Position of the vertex)
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO[0]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);

  //Color
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

  glBindVertexArray(0);

  // Create a shader using my GLSLObject class
  sivelab::GLSLObject shader;
  shader.addShader( "vertexShader_PrepForPerFragment.glsl", sivelab::GLSLObject::VERTEX_SHADER );
  shader.addShader( "fragmentShader_BlinnPhong.glsl", sivelab::GLSLObject::FRAGMENT_SHADER );
  shader.createProgram();

  GLuint projMatrixID, viewMatrixID, modelMatrixID, normalMatrixID;
  projMatrixID = shader.createUniform("projMatrix");
  viewMatrixID = shader.createUniform("viewMatrix");
  modelMatrixID = shader.createUniform("modelMatrix");
  normalMatrixID = shader.createUniform("normalMatrix");


  // Blinn-Phong
  GLuint lightPosID = shader.createUniform("lightPosWorld");
  GLuint intensityID = shader.createUniform("intensity");
  GLuint laID = shader.createUniform("la");
  GLuint kaID = shader.createUniform("ka");
  GLuint kdID = shader.createUniform("kd");
  GLuint ksID = shader.createUniform("ks");
  GLuint phongExpID = shader.createUniform("phongExp");

  //Light data
  glm::vec4 lightPosWorld(0.0f, 0.0f, 3.0f, 1.0f);
  glm::vec3 intensity(1.0f, 1.0f, 1.0f);

  //Blinn-Phong parameters
  glm::vec3 la(0.2f, 0.2f, 0.2f);
  glm::vec3 ka(0.2f, 0.2f, 0.2f);
  glm::vec3 kd(0.8f, 0.4f, 0.2f);  // orange-ish
  glm::vec3 ks(1.0f, 1.0f, 1.0f);  // white specular
  float phongExp = 32.0f;

  shader.activate();
  glUniform4fv(lightPosID,1, glm::value_ptr(lightPosWorld));
  glUniform3fv(intensityID,1, glm::value_ptr(intensity));
  glUniform3fv(laID,1, glm::value_ptr(la));
  glUniform3fv(kaID,1, glm::value_ptr(ka));
  glUniform3fv(kdID,1, glm::value_ptr(kd));
  glUniform3fv(ksID,1, glm::value_ptr(ks));
  glUniform1f(phongExpID, phongExp);
  shader.deactivate();

  glm::mat4 modelTransform = glm::mat4(1.0f);
  float rot = 0;
  modelTransform = glm::rotate(modelTransform, rot, glm::vec3(0.0f, 1.0f, 0.0f));

  glm::vec3 m_pos(0,0,10), m_viewDir(0,0,-1);
  glm::vec3 m_U(1,0,0), m_V(0,1,0), m_W(0,0,1);

    double timeDiff = 0.0, startFrameTime = 0.0, endFrameTime = 0.0;
    
    /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    endFrameTime = glfwGetTime();
    timeDiff = endFrameTime - startFrameTime;
    startFrameTime = glfwGetTime();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 M_view = glm::lookAt(m_pos, m_pos - m_W, m_V);

    shader.activate();

    // Update model transform
    modelTransform = glm::mat4(1.0f);
    modelTransform = glm::rotate(modelTransform, rot, glm::vec3(0, 1, 0));
    rot += 0.0001f;
    if (rot > 3.14159f * 2) rot = 0.0f;

    // Compute normal matrix from model matrix
    glm::mat4 normalMatrix = glm::transpose(glm::inverse(M_view * modelTransform));

    // Set all uniforms
    glUniformMatrix4fv(projMatrixID,   1, GL_FALSE, glm::value_ptr(perspMat));
    glUniformMatrix4fv(viewMatrixID,   1, GL_FALSE, glm::value_ptr(M_view));
    glUniformMatrix4fv(modelMatrixID,  1, GL_FALSE, glm::value_ptr(modelTransform));
    glUniformMatrix4fv(normalMatrixID, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    // Draw List of Triangles
    glBindVertexArray(triVAO);
    glDrawArrays(GL_TRIANGLES, 0, vbo_listOfTriangles.size() / 6);
    glBindVertexArray(0);

    // Draw Triangle
    glm::mat4 triangleModel = glm::mat4(1.0f);
    triangleModel = glm::rotate(triangleModel, rot, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 triangleNormal = glm::transpose(glm::inverse(M_view * triangleModel));
    glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(triangleModel));
    glUniformMatrix4fv(normalMatrixID, 1, GL_FALSE, glm::value_ptr(triangleNormal));
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Draw Sphere
    glm::mat4 sphereModel = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 0.0f));
    glm::mat4 sphereNormal = glm::mat4(glm::transpose(glm::inverse(M_view * glm::mat4(sphereModel))));
    glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(sphereModel));
    glUniformMatrix4fv(normalMatrixID, 1, GL_FALSE, glm::value_ptr(sphereNormal));
    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    shader.deactivate();

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
