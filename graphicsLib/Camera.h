//
// Created by Evan on 1/28/2026.
//

#ifndef CS4212STARTERCODE_CAMERA_H
#define CS4212STARTERCODE_CAMERA_H

#include "ray.h"
#include "vec3.h"

class Camera
{
  public:
    Camera() : pos(0,0,0), U(1,0,0), V(0,1,0), W(0,0,1), focalLength(1.0),
        imagePlane_width(0.25), imagePlane_height(0.25), nx(100), ny(100) {}

    Camera(int pixel_nx, int pixel_ny) : pos(0,0,0), U(1,0,0), V(0,1,0), W(0,0,1), focalLength(1.0),
        imagePlane_width(0.25), imagePlane_height(0.25), nx(pixel_nx), ny(pixel_ny) {}

    virtual ~Camera() {}


    //Camera needs to know pixel image dimensions
    virtual void generateRay(int i, int j, ray &r) = 0;

  protected:
    vec3 pos;

    vec3 U, V, W;

    float focalLength;
    float imagePlane_width, imagePlane_height;
    float left_bound, right_bound, top_bound, bottom_bound;

    int nx, ny;
private:
  vec3 location;
  vec3 direction;
};


#endif// CS4212STARTERCODE_CAMERA_H
