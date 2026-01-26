//
// Created by Evan on 1/22/2026.
//


#include "Framebuffer.h"

Framebuffer::Framebuffer() : width(100), height(100), fbStorage(width * height){}
Framebuffer::Framebuffer(int w, int h) : width(w), height(h), fbStorage(width * height){}

void Framebuffer::clearToColor(color c) {
  for (auto i=0u; i < fbStorage.size(); i++) {
      fbStorage[i] = c;
  }
}

void Framebuffer::makeGradient(vec3 t, vec3 b)
{
  for (int y = 0; y < height; y++)
  {
    double p = double(y) / (height - 1);
    color c(
        (1 - p) * t.x() + p * b.x(),
        (1 - p) * t.y() + p * b.y(),
        (1 - p) * t.z() + p * b.z()
    );

    for (int x = 0; x < width; x++)
      fbStorage[y * width + x] = c;
  }
}


void Framebuffer::setPixelColor(int x, int y, color c)
{
  fbStorage[y * width + x] = c;
}

void Framebuffer::setPixelColor(int index, color c)
{
  fbStorage[index] = c;
}

void Framebuffer::exportAsPNG(std::string filename)
{
  png::image<png::rgb_pixel> img(width, height);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {

      color c = fbStorage[y * width + x];

      int r = std::clamp(int(c.x()), 0, 255);
      int g = std::clamp(int(c.y()), 0, 255);
      int b = std::clamp(int(c.z()), 0, 255);

      img[y][x] = png::rgb_pixel(r, g, b);
    }
  }

  img.write(filename);
}

