//
// Created by Evan on 2/5/2026.
//

#ifndef CS4212STARTERC ODE_TRIANGLE_H
#define CS4212STARTERCODE_TRIANGLE_H
#include "Shape.h"
#include "ray.h"
#include "vec3.h"

class Triangle : public Shape
{
  private:
    point3 vertex_a, vertex_b, vertex_c;

  public:
  Triangle() : vertex_a(point3(1,0,0)), vertex_b(point3(0, 1, 0)), vertex_c(point3(0, 0, 1)) {};
  Triangle(point3 a, point3 b, point3 c) : vertex_a(a), vertex_b(b), vertex_c(c) {};

  bool intersect(const ray &r, const float tmin, float &tmax) override
  {
    double a = vertex_a[0] - vertex_b[0];
    double b = vertex_a[1] - vertex_b[1];
    double c = vertex_a[2] - vertex_b[2];

    double d = vertex_a[0] - vertex_c[0];
    double e = vertex_a[1] - vertex_c[1];
    double f = vertex_a[2] - vertex_c[2];

    double g = r.direction()[0];
    double h = r.direction()[1];
    double i = r.direction()[2];

    double j = vertex_a[0] - r.origin()[0];
    double k = vertex_a[1] - r.origin()[1];
    double l = vertex_a[2] - r.origin()[2];

    double ei_minus_hf = e*i - h*f;
    double gf_minus_di = g*f - d*i;
    double dh_minus_eg = d*h - e*g;
    double ak_minus_jb = a*k - j*b;
    double jc_minus_al = j*c - a*l;
    double bl_minus_kc = b*l - k*c;

    double M = a * ei_minus_hf + b * gf_minus_di + c * dh_minus_eg;

    if (fabs(M) < 1e-8) return false;

    float t = -(f*ak_minus_jb + e*jc_minus_al + d*bl_minus_kc) / M;

    if (t < tmin)
      return false;

    double gamma = (i*ak_minus_jb + h*jc_minus_al + g*bl_minus_kc) / M;
    if (gamma < 0.0 || gamma > 1.0)
      return false;

    double beta = (j*ei_minus_hf + k*gf_minus_di + l*dh_minus_eg) / M;
    if (beta < 0.0 || beta > 1.0 - gamma)
      return false;

    tmax = t;
    return true;
  }
};

#endif// CS4212STARTERCODE_TRIANGLE_H
