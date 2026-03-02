//
// Created by Evan on 2/19/2026.
//
#include "Scene.h"

#include <random>

void Scene::exportScene(std::string name) { fb.exportAsPNG(name); }

color Scene::computeRaycolor(const Ray &r, float tmin, float tmax, int depth){
  if (depth <= 0) {
    return bgColor;
  }

  hitStructure closestHit;

  for (auto &s : allShapes) {
    hitStructure tempHit;
    float t_hit = tmax;

    bool hit = s->intersect(r, tmin, t_hit, tempHit);

    if (hit && t_hit < tmax) {
      tmax = t_hit;
      closestHit = tempHit;
      closestHit.hitShape = s;
      closestHit.r = r;
      closestHit.shader = tempHit.shader;
    }
  }


  if (closestHit.shader) {
    color c = closestHit.shader->rayColor(closestHit, depth);
    return c;
  } else {
    return bgColor;
  }
}

void Scene::generateScene()   {
  p.lookAt(vec3(0,0,1));
  int rpp_NSquare = 8;
  int depth = 10;

  for (int x = 0; x < fb.getWidth(); ++x) {
    for (int y = 0; y < fb.getHeight(); ++y) {
      color c(0.0, 0.0, 0.0);

      for (int pp =0; pp < rpp_NSquare; ++pp) {
        for (int q = 0; q < rpp_NSquare; ++q) {
          float tmin = 0.001f;
          float tmax = std::numeric_limits<float>::infinity();

          float pOffset = (pp + randomOffset())/rpp_NSquare;
          float qOffset = (q + randomOffset())/rpp_NSquare;

          Ray r;
          p.generateRay(x+pOffset, y+qOffset, r);

          //std::cout << "ORIGIN: " << r.origin() << " DIRECTION: " << r.direction() << std::endl;

          c += computeRaycolor(r, tmin, tmax, depth);
        }
      }

      c = c / (float)(rpp_NSquare * rpp_NSquare);
      fb.setPixelColor(x, y, c);
    }
  }
}

void Scene::addShape(const std::shared_ptr<Shape> shapePtr) {
  allShapes.push_back(shapePtr);
}

void Scene::addLight(const std::shared_ptr<Light> lightPtr)
{
  allLights.push_back(lightPtr);
}

std::vector<std::shared_ptr<Light>> Scene::getLights() {
  return allLights;
}
std::vector<std::shared_ptr<Shape>> Scene::getShapes() {
  return allShapes;
};

vec3 Scene::getEyePosition()
{
  return p.getEyePosition();
}

void Scene::moveTo(const point3 &vec3) {
  p.moveTo(vec3);
}

void Scene::lookAt(const point3 &vec3)
{
  p.lookAt(vec3);
}
void Scene::setBG(const color &c) {
  bgColor = c;
}

float Scene::randomOffset() {
  static std::uniform_real_distribution<double> distribution(0.0, 1.0);
  static std::mt19937 generator;
  return distribution(generator);
}