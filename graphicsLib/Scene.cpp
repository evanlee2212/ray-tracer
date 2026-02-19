//
// Created by Evan on 2/19/2026.
//
#include "Scene.h"

void Scene::exportScene(std::string name) { fb.exportAsPNG(name); }

color Scene::computeRaycolor(const Ray &r, float tmin, float tmax, int depth){
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

void Scene::generateScene()   {
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
        fb.setPixelColor(x, y, bgColor);
      }
    }
  }
}

void Scene::addShape(const std::shared_ptr<Shape> shapePtr) {
  allShapes.push_back(shapePtr);
}