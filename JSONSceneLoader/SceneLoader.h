#pragma once

#include "ISceneLoader.h"
#include "../graphicsLib/Scene.h"
#include "../graphicsLib/PointLight.h"

class SceneLoader : public ISceneLoader {
private:
  Scene &m_targetScene; // Reference to the external scene

public:
  // The caller provides the scene to be filled
  SceneLoader(Scene &sceneToPopulate)
      : m_targetScene(sceneToPopulate), numShaders(0), numTextures(0) {}

  void reserveCameras(size_t count) override {
    // makes sure the vector has enough space
    m_targetScene.getCameras().reserve(count);
  }

  void reserveLights(size_t count) override {
    m_targetScene.getLights().reserve(count);
  }

  void reserveShapes(size_t count) override {
    m_targetScene.getShapes().reserve(count);
  }

  void reserveShaders(size_t count) override { numShaders = count; }
  void reserveTextures(size_t count) override { numTextures = count; }

  void setSceneParams(const SceneParams &sparams) override {
    std::cout << "Setting scene params." << std::endl;
  }

  std::shared_ptr<Camera> createCamera(const std::string& type)
  {
    if (type == "perspective") {
      return std::make_shared<PerspectiveCamera>(); //Needs dimensions
    } else throw std::invalid_argument("Invalid camera: " + type);
  }

  void addCamera(const std::string &name, const std::string &type,
                 const ISceneLoader::vec &pos, const ISceneLoader::vec &viewDir,
                 float focalLength, float imagePlaneWidth) override {
    std::cout << "Creating camera: " << name << ", type: " << type << std::endl;
    auto cam = createCamera(type);
    m_targetScene.addCamera(cam);
  }

  void addPointLight(const vec pos, const vec intensity) override {
    std::cout << "Creating point light." << std::endl;

    auto light = std::make_shared<PointLight>(
      vec3(pos.x, pos.y, pos.z),
      vec3(intensity.x, intensity.y, intensity.z),
      1.0f);

    m_targetScene.addLight(light);
  }

  void addAreaLight(const ISceneLoader::vec &pos,
                    const ISceneLoader::vec &color,
                    const ISceneLoader::vec &normal, float width,
                    float length) override {
    std::cout << "Creating area light." << std::endl;
  }
  void addShader(const ISceneLoader::ShaderDesc &shaderDesc) override {
    std::cout << "Creating shader: type=" << shaderDesc.type << std::endl;
    // I place my shaders on an unordered map often
  }
  void addShape(const ISceneLoader::ShapeDesc &shapeDesc) override {
    std::cout << "Creating shape: type=" << shapeDesc.type << std::endl;
  }
  void addTexture(const std::string &type, const std::string &name,
                  const std::string &sourceFile) override {
    std::cout << "Creating texture: type=" << type << ", name=" << name
              << ", sourceFile=" << sourceFile << std::endl;
  }

public:
  int numShaders;
  int numTextures;
};