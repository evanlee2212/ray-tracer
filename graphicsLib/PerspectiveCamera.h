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
  PerspectiveCamera(int w, int h) : Camera(w,h) { left_bound = -imagePlane_width/2.0f, right_bound = imagePlane_width/2.0f,
              top_bound = imagePlane_height/2.0f, bottom_bound = -imagePlane_height/2.0f; }

  void generateRay(float i, float j, Ray &r) override;
  vec3 getEyePosition();

  std::string type = "Perspective";
};

#endif// CS4212STARTERCODE_PERSPECTIVECAMERA_H
