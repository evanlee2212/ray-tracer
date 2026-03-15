#pragma once

#include "ISceneLoader.h"
#include "../graphicsLib/Scene.h"
#include "../graphicsLib/PointLight.h"
#include "../graphicsLib/LambertianShader.h"
#include "../graphicsLib/BlinnPhongShader.h"
#include "../graphicsLib/MirrorShader.h"
#include "../graphicsLib/Sphere.h"
#include "../graphicsLib/Triangle.h"

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
    std::cout << "  pos: " << pos.x << "," << pos.y << "," << pos.z << std::endl;
    std::cout << "  viewDir: " << viewDir.x << "," << viewDir.y << "," << viewDir.z << std::endl;
    std::cout << "  focalLength: " << focalLength << std::endl;
    std::cout << "  imagePlaneWidth: " << imagePlaneWidth << std::endl;

    auto cam = createCamera(type);

    //position
    cam->moveTo(vec3(pos.x, pos.y, pos.z));

    //Look direction
    vec3 target = vec3(pos.x + viewDir.x, pos.y + viewDir.y, (pos.z + viewDir.z));
    cam->lookAt(target);

    //Set focal length and image plane
    cam->setFocalLength(focalLength);
    cam->setImagePlaneWidth(imagePlaneWidth);


    m_targetScene.addCamera(cam);
  }

  void addPointLight(const vec pos, const vec intensity) override {
    std::cout << "Creating point light." << std::endl;

    auto light = std::make_shared<PointLight>(
      vec3(pos.x, pos.y, pos.z),
      vec3(intensity.x, intensity.y, intensity.z) * 255,
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
    //Make Shader
    std::shared_ptr<Shader> shader;

    if (shaderDesc.type == "Lambertian") {
      shader = std::make_shared<LambertianShader>(m_targetScene);
    } else if (shaderDesc.type == "BlinnPhong") {
      shader = std::make_shared<BlinnPhongShader>(m_targetScene);
    } else if (shaderDesc.type == "Mirror") {
      shader = std::make_shared<MirrorShader>(m_targetScene);
    } else throw std::invalid_argument("Invalid shader type: " + shaderDesc.type);

    // I place my shaders on an unordered map often
    m_targetScene.shaderMap.emplace(shaderDesc.name, shader);
  }

  void addShape(const ISceneLoader::ShapeDesc &shapeDesc) override {
    std::cout << "Creating shape: type=" << shapeDesc.type << std::endl;

    std::cout << "addShape called: type=" << shapeDesc.type
              << " shader=" << shapeDesc.shaderNameReference
              << " center=" << shapeDesc.center.x << "," << shapeDesc.center.y << "," << shapeDesc.center.z
              << " radius=" << shapeDesc.radius << std::endl;

    std::shared_ptr<Shape> shape;
    std::shared_ptr<Shader> shader;

    //Find Shader
    std::cout << "Looking for shader: " << shapeDesc.shaderNameReference << std::endl;
    std::cout << "Shaders in map: ";
    for (auto &s : m_targetScene.shaderMap) std::cout << s.first << " ";
    std::cout << std::endl;

    auto it = m_targetScene.shaderMap.find(shapeDesc.shaderNameReference);
    if (it != m_targetScene.shaderMap.end()) {
      std::cout << "Shader found, linking." << std::endl;
      shader = it->second;
    } else {
      std::cerr << "Shader not found: " << shapeDesc.shaderNameReference << std::endl;
    }

    if (shapeDesc.type == "sphere") {
      vec3 center(shapeDesc.center.x, shapeDesc.center.y, shapeDesc.center.z);
      shape = std::make_shared<Sphere>(center, shapeDesc.radius, shader, color(0, 0, 1)*255);
      std::cout << "Sphere created at " << center.x() << "," << center.y() << "," << center.z() << std::endl;

    } else if (shapeDesc.type == "triangle") {
      vec3 v0(shapeDesc.v0.x, shapeDesc.v0.y, shapeDesc.v0.z);
      vec3 v1(shapeDesc.v1.x, shapeDesc.v1.y, shapeDesc.v1.z);
      vec3 v2(shapeDesc.v2.x, shapeDesc.v2.y, shapeDesc.v2.z);
      shape = std::make_shared<Triangle>(v0, v1, v2, shader, color(0, 0, 1)*255);
    } else {
      throw std::invalid_argument("Unknown shape type: " + shapeDesc.type);
    }

    m_targetScene.addShape(shape);
    std::cout << "Shape added. Total shapes: " << m_targetScene.getShapes().size() << std::endl;
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