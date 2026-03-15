//
// Created by Evan on 2/10/2026.
//

#pragma once
#include "Framebuffer.h"
#include "PerspectiveCamera.h"
#include "Shader.h"
#include "Shape.h"
#include "Ray.h"
#include "Light.h"

#include <cfloat>
#include <memory>
#include <vector>

struct hitStructure
{
  float t = 0.0f;
  vec3 normal;
  point3 point;
  std::shared_ptr<Shape> hitShape = nullptr;
  std::shared_ptr<Shader> shader = nullptr;
  color color;
  Ray r;
};


class Scene {
public:
  Scene() : fb(100,100), p(100, 100) {}
  Scene(int width, int height) : fb(width, height), p(width, height) {};

  void exportScene(std::string name);

  void addShape( const std::shared_ptr<Shape> shapePtr );

  color computeRaycolor( const Ray &r, float tmin, float tmax , int depth);

  void addLight ( const std::shared_ptr<Light> lightPtr );

  std::vector<std::shared_ptr<Light>> getLights();

  std::vector<std::shared_ptr<Shape>> getShapes();

  void addCamera( const std::shared_ptr<Camera> camPtr );

  std::vector<std::shared_ptr<Camera>> getCameras();

  void generateScene();

  vec3 getEyePosition();

  void moveTo(const point3 & vec3);
  void lookAt(const point3 & vec3);
  void setBG(const color & c);

private:
  std::vector<std::shared_ptr<Shape>> allShapes;
  color bgColor = color(135, 206, 235);
  Framebuffer fb;
  PerspectiveCamera p = PerspectiveCamera(fb.getWidth(), fb.getHeight());
  std::vector<std::shared_ptr<Light>> allLights;
  std::vector<std::shared_ptr<Camera>> allCameras;

  float randomOffset();
};
