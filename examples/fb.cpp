//
// Created by Evan on 1/26/2026.
//

#include "../graphicsLib/Framebuffer.h"

int main()
{
  Framebuffer fb(100, 100);

  fb.clearToColor(color(0, 255, 255));
  fb.exportAsPNG("cyan.png");

  fb.makeGradient(color(255, 0,0), color(0, 0, 255));
  fb.exportAsPNG("redBlueGrad.png");

  return 0;
}