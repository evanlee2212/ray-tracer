//
// Created by Evan on 1/26/2026.
//

#include "../graphicsLib/Framebuffer.h"
#include "../graphicsLib/PerspectiveCamera.h"
#include "../graphicsLib/Sphere.h"
#include "../graphicsLib/Triangle.h"

#include <cfloat>

int main()
{
  Framebuffer fb(250, 250);
  PerspectiveCamera p(fb.getWidth(), fb.getHeight());

  std::vector<std::unique_ptr<Shape>> shapes;

    shapes.push_back(std::make_unique<Triangle>(
        point3(-1.2, -0.2, -7),
        point3(0.8, -0.5, -5),
        point3(0.9, 0, -5)));

    shapes.push_back(std::make_unique<Triangle>(
        point3(0.773205, -0.93923, -7),
        point3(0.0330127, 0.94282, -5),
        point3(-0.45, 0.779423, -5)));

    shapes.push_back(std::make_unique<Triangle>(
        point3(0.426795, 1.13923, -7),
        point3(-0.833013, -0.44282, -5),
        point3(-0.45, -0.779423, -5)));

  for (int x = 0; x < fb.getWidth(); ++x) {
    for (int y = 0; y < fb.getHeight(); ++y) {

      ray r;
      p.generateRay(x, y, r);

      float t_closest = FLT_MAX;
      const Shape* hit_shape = nullptr;

      for (const auto& s : shapes) {
        float t_hit;
        if (s->intersect(r, 0.001f, t_hit) && t_hit < t_closest) {
          t_closest = t_hit;
          hit_shape = s.get();
        }
      }

      color c = hit_shape ? color(255, 255, 255)
                          : color(0, 0, 0);

      fb.setPixelColor(x, y, c);
    }
  }

  fb.exportAsPNG("TriangleTest.png");
}
