//
// Created by Evan on 1/28/2026.
//

#ifndef CS4212STARTERCODE_CAMERA_H
#define CS4212STARTERCODE_CAMERA_H

#include "Ray.h"
#include "vec3.h"

class Camera
{
  public:
    Camera() : pos(0,0,0), U(1,0,0), V(0,1,0), W(0,0,-1), focalLength(0.5),
        imagePlane_width(0.5), imagePlane_height(0.5), nx(100), ny(100) {}

    Camera(int pixel_nx, int pixel_ny) : pos(0,0,0), U(1,0,0), V(0,1,0), W(0,0,-1), focalLength(0.5),
        imagePlane_width(0.25), imagePlane_height(0.25), left_bound(-0.125), right_bound(0.125), bottom_bound(0.125), top_bound(0.125),
        nx(pixel_nx), ny(pixel_ny) {}

    virtual ~Camera() {}

    void moveTo(point3 e) { pos = e; }
    void lookAt(point3 target, vec3 up=vec3(0,1,0)) {
      W = unit_vector(pos - target);
      U = unit_vector(cross(up, W));
      V = cross(W, U);
    }


    //Camera needs to know pixel image dimensions
    virtual void generateRay(float i, float j, Ray &r) = 0;
    virtual std::string getType() { return type; }
    virtual void setType(std::string type) { this->type = type; }

    std::string type;

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
