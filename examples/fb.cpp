//
// Created by Evan on 1/26/2026.
//

#include "../graphicsLib/Framebuffer.h"
#include "../graphicsLib/PerspectiveCamera.h"

int main()
{
  Framebuffer fb(100, 100);

  /**fb.clearToColor(color(0, 255, 255));
  fb.exportAsPNG("cyan.png");

  fb.makeGradient(color(255, 0,0), color(0, 0, 255));
  fb.exportAsPNG("redBlueGrad.png");
  **/

  PerspectiveCamera p;

  for (int x = 0; x < 100; ++x) {
    for (int y = 0; y < 100; ++y) {

      ray r;
      p.generateRay(x, y, r);

      vec3 d = unit_vector(r.direction());

      color c(
        (d.x() + 1.0f) * 0.5f * 255,
        (d.y() + 1.0f) * 0.5f * 255,
        (d.z() + 1.0f) * 0.5f * 255
      );

      fb.setPixelColor(x, y, c);
    }
  }

  fb.exportAsPNG("defaultCamRayColors.png");
  return 0;
}