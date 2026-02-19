//
// Created by Evan on 2/10/2026.
//

#pragma once
#include "Framebuffer.h"
#include "PerspectiveCamera.h"
#include "Shader.h"
#include "Shape.h"
#include "Ray.h"

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

  void generateScene();

private:
  std::vector<std::shared_ptr<Shape>> allShapes;
  color bgColor = color(135, 206, 235);
  Framebuffer fb;
  PerspectiveCamera p = PerspectiveCamera(fb.getWidth(), fb.getHeight());


};
