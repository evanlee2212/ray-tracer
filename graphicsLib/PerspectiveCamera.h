//
// Created by Evan on 1/29/2026.
//

#ifndef CS4212STARTERCODE_PERSPECTIVECAMERA_H
#define CS4212STARTERCODE_PERSPECTIVECAMERA_H
#include "Camera.h"

class PerspectiveCamera : public Camera
{
public:
  PerspectiveCamera() : Camera() { left_bound = -imagePlane_width/2.0f, right_bound = imagePlane_width/2.0f,
              top_bound = imagePlane_height/2.0f, bottom_bound = -imagePlane_height/2.0f; };

  void generateRay(int i, int j, ray &r) override
  {
    float u, v;
    //im = new image(nx * ny * 3)

    for (int pi = 0; pi < imagePlane_width; ++pi) {
      //Compute Viewing Ray
      u = left_bound + (right_bound - left_bound) * (i + 0.5)/(float)nx;
      v = bottom_bound + (top_bound - bottom_bound) * (j + 0.5)/(float)ny;

      ray tempRay( pos, -W*focalLength + U*u + V*v);
      r = tempRay;

      //intersect with Objects
      //object = findFirstObject(r);

      //set pixel color based on object shader
      //im[pi * pj] = shader(object);
    }
  }
};

#endif// CS4212STARTERCODE_PERSPECTIVECAMERA_H
