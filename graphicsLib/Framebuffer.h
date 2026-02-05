//
// Created by Evan on 1/22/2026.
//
#ifndef CS4212STARTERCODE_FRAMEBUFFER_H
#define CS4212STARTERCODE_FRAMEBUFFER_H

#pragma once

#include <vector>
#include <fstream>

#include "../src/png++/png.hpp"
#include "../src/handleGraphicsArgs.h"

#include "vec3.h"

using color = vec3;

class Framebuffer {
public:
  Framebuffer();
  Framebuffer(int w, int h);

  void clearToColor( vec3 c );
  void makeGradient(vec3 t, vec3 b);

  void setPixelColor( int i, int j, color c);
  void setPixelColor( int index, color c);

  void exportAsPNG(std::string filename);

  int getWidth();
  int getHeight();

private:
  int width, height;
  std::vector<vec3> fbStorage;
};


#endif// CS4212STARTERCODE_FRAMEBUFFER_H