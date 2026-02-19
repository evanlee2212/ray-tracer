//
// Created by Evan on 1/26/2026.
//

#include "../graphicsLib/Scene.h"
#include "../graphicsLib/BlinnPhongShader.h"
#include "../graphicsLib/Framebuffer.h"
#include "../graphicsLib/LambertianShader.h"
#include "../graphicsLib/PerspectiveCamera.h"
#include "../graphicsLib/Sphere.h"
#include "../graphicsLib/Triangle.h"

int main()
{
    Scene world;

    auto lambertianRed   = std::make_shared<LambertianShader>();
    auto lambertianGreen = std::make_shared<LambertianShader>();
    auto lambertianBlue  = std::make_shared<LambertianShader>();
    auto blinnPhong = std::make_shared<BlinnPhongShader>();

    std::vector<std::shared_ptr<Shape>> shapes;

    world.addShape(std::make_shared<Triangle>(
      point3(0.9, 0, -5),
      point3(0.8, -0.5, -5),
      point3(-1.2, -0.2, -7),
        lambertianRed,
        color(255, 0, 0)
    ));

    world.addShape(std::make_shared<Triangle>(
        point3(0.773205, -0.93923, -7),
        point3(0.0330127, 0.94282, -5),
        point3(-0.45, 0.779423, -5),
        lambertianGreen,
        color(0, 255, 0)
    ));

    world.addShape(std::make_shared<Triangle>(
        point3(-0.45, -0.779423, -5),
        point3(-0.833013, -0.44282, -5),
        point3(0.426795, 1.13923, -7),
        lambertianBlue,
        color(0, 0, 255)
    ));

  world.addShape(std::make_shared<Sphere>(
    point3(0, 0, -10),
      2.0f,
      lambertianRed,
      color(255, 0, 0)
  ));

  world.addShape(std::make_shared<Sphere>(
    point3(0, 0, -5),
    0.25f,
    blinnPhong,
    color(100, 100, 100)));

  world.generateScene();

  world.exportScene("ShaderTest.png");

  return 0;

};
