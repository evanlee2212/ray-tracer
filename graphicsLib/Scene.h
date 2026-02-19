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
  void exportScene(std::string name) { fb.exportAsPNG(name); }

  void addShape( const std::shared_ptr<Shape> shapePtr ) {
    allShapes.push_back(shapePtr);
  };

  color computeRaycolor( const Ray &r, float tmin, float tmax , int depth)
  {
    if (depth <= 0) { return bgColor; }

    hitStructure h;
    float localTmax = tmax;

    bool hitShape = false;
    for (int i = 0; i < allShapes.size(); ++i) {
      if (allShapes[i]->intersect(r, tmin, localTmax, h)) {
        hitShape = true;
      }
    }

    if (hitShape) {
      std::shared_ptr<Shader> shader = h.shader;
      color c = shader->rayColor(h, depth-1);
      return c;
    } else {
      return bgColor;
    }
  }

  void generateScene()
  {
    p.lookAt(vec3(0,0,1));
    int rpp_NSquare = 4;
    for (int x = 0; x < fb.getWidth(); ++x) {
      for (int y = 0; y < fb.getHeight(); ++y) {
        Ray r;
        p.generateRay(x, y, r);

        hitStructure closestHit;
        float t_closest = FLT_MAX;

        for (auto &s : allShapes) {
          hitStructure tempHit;
          float t_hit;
          if (s->intersect(r, 0.0f, t_hit, tempHit) && t_hit < t_closest) {
            t_closest = t_hit;
            closestHit = tempHit;
            closestHit.hitShape = s;
            closestHit.r = r;
          }
        }

        if (closestHit.shader) {
          fb.setPixelColor(x, y, closestHit.shader->rayColor(closestHit, 10));
        } else {
          fb.setPixelColor(x, y, color(0, 150, 150));
        }
      }
    }
  }

private:
  std::vector<std::shared_ptr<Shape>> allShapes;
  color bgColor = color(100, 200, 200);
  Framebuffer fb = Framebuffer(250, 250);
  PerspectiveCamera p = PerspectiveCamera(fb.getWidth(), fb.getHeight());


};
