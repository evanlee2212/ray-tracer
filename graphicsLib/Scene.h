//
// Created by Evan on 2/10/2026.
//

#pragma once
#include "Framebuffer.h"
#include "Shape.h"

#include <memory>
#include <vector>

struct HitRecord {
  //Need details on what happened at the surface!!
};

class Scene {
public:
  void addShape( const std::shared_ptr<<Shape> shape_ptr );

  color rayColor(const ray& r)
  {
    /**
     *for (all shapes, s) {
     *  s->intersect(r, tmin, tmax);
     * }
     *
     * Find closest shape
     * Get its shader
     *
     * if (hitSomething) {
     *  color c = shader->getRayColor(HitStructure);
     * } else {
     *  color c = backgroundColor;
     * }
     */
  };
private:
  std::vector<std::shared_ptr<Shape>> allShapes;

  //Require more class implementation
  //std::map< std::string, std::shared_ptr<Shaders> > allShaders;
  //std::vector< std::shared_ptr<Light> allLights;


};
