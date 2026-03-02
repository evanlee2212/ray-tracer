//
// Created by Evan on 2/19/2026.
//
#include "Triangle.h"

bool Triangle::intersect(const Ray &r, const float tmin, float &tmax, hitStructure &hitStruct){
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

  double t = -(f*ak_minus_jb + e*jc_minus_al + d*bl_minus_kc) / M;

  if (t < tmin)
    return false;

  double gamma = (i*ak_minus_jb + h*jc_minus_al + g*bl_minus_kc) / M;
  if (gamma < 0.0 || gamma > 1.0)
    return false;

  double beta = (j*ei_minus_hf + k*gf_minus_di + l*dh_minus_eg) / M;
  if (beta < 0.0 || beta > 1.0 - gamma)
    return false;

  if (t < tmax) {
    tmax = t;

    //Calculating Normal
    vec3 u = vertex_b - vertex_a;
    vec3 v = vertex_c - vertex_a;

    //Update hitStructure
    hitStruct.t = tmax;
    hitStruct.point = r.at(tmax);
    hitStruct.normal = unit_vector(cross(u, v));
    hitStruct.shader = getShader();
    hitStruct.color = _color;

    return true;
  }

  return false;
}

color Triangle::getColor() {
  return _color;
}

std::shared_ptr<Shader> Triangle::getShader() { return shader; }