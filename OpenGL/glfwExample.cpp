#include <cstdlib>
#include <iostream>
#include <vector>

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

  //Load triangle
  GLuint m_triangleVBO[1], m_VAO;

  glGenBuffers(1, m_triangleVBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO[0]);

  std::vector<float> host_VertexBuffer{
    // Position           //Normal
    -3.0f, -3.0f, 0.0f,   0.0f, 0.0f, 1.0f,
    3.0f, -3.5f, 0.0f,   0.0f, 0.0f, 1.0f,
    0.0f, 3.0f, 0.0f,    0.0f, 0.0f, 1.0f,};

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
  shader.addShader( "fragmentShader_Normal.glsl", sivelab::GLSLObject::FRAGMENT_SHADER );
  shader.createProgram();

  GLuint projMatrixID, viewMatrixID, modelMatrixID, normalMatrixID;
  projMatrixID = shader.createUniform("projMatrix");
  viewMatrixID = shader.createUniform("viewMatrix");
  modelMatrixID = shader.createUniform("modelMatrix");
  normalMatrixID = shader.createUniform("normalMatrix");

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

    // Draw
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    shader.deactivate();

    glfwSwapBuffers(window);
    glfwPollEvents();

    // Camera movement
    float moveRatePerFrame = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      m_pos = m_pos + -m_W * moveRatePerFrame;
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      m_pos = m_pos - m_U * moveRatePerFrame;
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      m_pos = m_pos + m_W * moveRatePerFrame;
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      m_pos = m_pos + m_U * moveRatePerFrame;

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
      std::cout << "fps: " << 1.0 / timeDiff << std::endl;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, 1);
  }
  
    glfwTerminate();
    return 0;
}
