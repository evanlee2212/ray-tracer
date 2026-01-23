//
// Created by Evan on 1/21/2026.
//

#ifndef CS4212STARTERCODE_VEC3_H
#define CS4212STARTERCODE_VEC3_H

#include <cmath>
#include <iostream>

class vec3 {
  public:
    double e[3];

    vec3() : e{0,0,0} {}
    vec3( double x, double y, double z ) : e{x,y,z} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }
};

#endif// CS4212STARTERCODE_VEC3_H
