//
// Created by Evan on 1/22/2026.
//

/**
#include "Framebuffer.h"

Framebuffer::Framebuffer() : width(100), height(100), fbStorage(width * height){}
Framebuffer::Framebuffer(int w, int h) : width(w), height(h), fbStorage(width * height){}

void Framebuffer::clearToColor(color c) {
  for (auto i=0u; i < fbStorage.size(); i++) {
      fbStorage[i] = c;
  }
}

/** void Framebuffer::makeGradient(vec3 t, vec3 b){
  double p = 0;

  for (int i = 0; i < fbStorage.size(); i++) {
    auto tx = t.x;
    auto x = (1-p)*t.x + p*b.x;
    fbStorage[i] = vec3((1-p)*t.x + p*b.x,
      (1-p)*t.y + p*b.y,
      (1-p)*t.z + p*b.z);
  }
} **/