//
// Created by Evan on 1/26/2026.
//

#include "../graphicsLib/Framebuffer.h"
#include "../graphicsLib/PerspectiveCamera.h"
#include "../graphicsLib/Sphere.h"

int main()
{
  Framebuffer fb(1000, 1000);

  PerspectiveCamera p(fb.getWidth(), fb.getHeight());

  Sphere s(vec3(-1, -1, -3), 0.75);
  Sphere s2(vec3(1,1,-3), 0.75);
  Sphere s3(vec3(0,0,-3), 0.5);


  for (int x = 0; x < fb.getWidth(); ++x) {
    for (int y = 0; y < fb.getHeight(); ++y) {
      ray r;
      p.generateRay(x, y, r);

      vec3 d = unit_vector(r.direction());
      color c;

      if (s.intersect(r) || s2.intersect(r) || s3.intersect(r)) {
        c = color(
        (-d.x() + 1.0f) * 0.5f * 255,
        (-d.y() + 1.0f) * 0.5f * 255,
        (-d.z() + 1.0f) * 0.5f * 255
        );
      } else {
        c = color(
        (d.x() + 1.0f) * 0.5f * 255,
        (d.y() + 1.0f) * 0.5f * 255,
        (d.z() + 1.0f) * 0.5f * 255
        );
      }

      fb.setPixelColor(x,y, c);
    }
  }


  fb.exportAsPNG("sphereTest.png");
  return 0;
}