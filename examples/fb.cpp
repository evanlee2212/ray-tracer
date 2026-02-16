//
// Created by Evan on 1/26/2026.
//

#include "../graphicsLib/Framebuffer.h"
#include "../graphicsLib/LambertianShader.h"
#include "../graphicsLib/PerspectiveCamera.h"
#include "../graphicsLib/Sphere.h"
#include "../graphicsLib/Triangle.h"

#include <cfloat>

int main()
{
    Framebuffer fb(250, 250);
    PerspectiveCamera p(fb.getWidth(), fb.getHeight());
    p.lookAt(vec3(0,0,1));

    auto lambertianRed   = std::make_shared<LambertianShader>();
    auto lambertianGreen = std::make_shared<LambertianShader>();
    auto lambertianBlue  = std::make_shared<LambertianShader>();

    std::vector<std::shared_ptr<Shape>> shapes;

    shapes.push_back(std::make_shared<Triangle>(
        point3(-1.2, -0.2, -7),
        point3(0.8, -0.5, -5),
        point3(0.9, 0, -5),
        lambertianRed,
        color(255, 0, 0)
    ));

    shapes.push_back(std::make_shared<Triangle>(
        point3(0.773205, -0.93923, -7),
        point3(0.0330127, 0.94282, -5),
        point3(-0.45, 0.779423, -5),
        lambertianGreen,
        color(0, 255, 0)
    ));

    shapes.push_back(std::make_shared<Triangle>(
        point3(-0.45, -0.779423, -5),
        point3(0.426795, 1.13923, -7),
        point3(-0.833013, -0.44282, -5),
        lambertianBlue,
        color(0, 0, 255)
    ));

  shapes.push_back(std::make_shared<Sphere>(
    point3(0, 0, -10),
      2.0f,
      lambertianRed,
      color(255, 0, 0)
  ));

  for (int x = 0; x < fb.getWidth(); ++x) {
    for (int y = 0; y < fb.getHeight(); ++y) {
      ray r;
      p.generateRay(x, y, r);

      hitStructure closestHit;
      float t_closest = FLT_MAX;

      for (auto& s : shapes) {
        hitStructure tempHit;
        float t_hit;
        if (s->intersect(r, 0.0f, t_hit, tempHit) && t_hit < t_closest) {
          t_closest = t_hit;
          closestHit = tempHit;
          closestHit.hitShape = s;
        }
      }

      if (closestHit.shader) {
        fb.setPixelColor(x, y, closestHit.shader->rayColor(closestHit));
      } else {
        fb.setPixelColor(x, y, color(0,0,0));
      }
    }
  }
  fb.exportAsPNG("ShaderTest.png");

  return 0;

};
