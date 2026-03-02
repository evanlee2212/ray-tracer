//
// Created by Evan on 2/19/2026.
//
#include "PerspectiveCamera.h"

#include "Scene.h"

void PerspectiveCamera::generateRay(float i, float j, Ray &r) {
  float u, v;
  //im = new image(nx * ny * 3)

  //Compute Viewing Ray
  u = left_bound + (right_bound - left_bound) * (i + 0.5f)/(float)nx;
  v = bottom_bound + (top_bound - bottom_bound) * (j + 0.5f)/(float)ny;

  vec3 dir = unit_vector(-W*focalLength + U*u + V*v);

  r = Ray(pos, dir);

}

vec3 PerspectiveCamera::getEyePosition()
{
  return pos;
}