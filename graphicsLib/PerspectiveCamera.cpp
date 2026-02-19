//
// Created by Evan on 2/19/2026.
//
#include "PerspectiveCamera.h"

#include "Scene.h"

void PerspectiveCamera::generateRay(float i, float j, Ray &r) {
  float u, v;
  //im = new image(nx * ny * 3)

  //Compute Viewing Ray
  u = left_bound + (right_bound - left_bound) * (i + 0.5)/(float)nx;
  v = bottom_bound + (top_bound - bottom_bound) * (j + 0.5)/(float)ny;

  vec3 dir = unit_vector(-W*focalLength + U*u + V*v);
  r = Ray(pos, dir);

  //intersect with Objects
  //object = findFirstObject(r);

  //set pixel color based on object shader
  //im[pi * pj] = shader(object);
}