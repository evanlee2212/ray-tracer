//
// Created by Evan on 2/22/2026.
//
#include "../graphicsLib/Scene.h"
#include "../graphicsLib/BlinnPhongShader.h"
#include "../graphicsLib/Framebuffer.h"
#include "../graphicsLib/LambertianShader.h"
#include "../graphicsLib/MirrorShader.h"
#include "../graphicsLib/PerspectiveCamera.h"
#include "../graphicsLib/Sphere.h"
#include "../graphicsLib/Triangle.h"

int main() {
  Scene world;

  auto lambertian  = std::make_shared<LambertianShader>();
  auto blinnPhong = std::make_shared<BlinnPhongShader>();
  auto mirror = std::make_shared<MirrorShader>(world);

  world.addShape(std::make_shared<Sphere>(
    point3(0, 50, -30),
    50.0f,
    lambertian,
    color(71, 40, 5)
    ));

  world.addShape(std::make_shared<Sphere>(
    point3(2, 0, -10),
      2.0f,
      lambertian,
      color(255, 0, 0)
  ));

  world.addShape(std::make_shared<Sphere>(
    point3(-1, 0, -10),
    1.0f,
    mirror,
    color(100, 100, 100)));

  world.generateScene();

  world.exportScene("MirrorTest.png");

  return 0;

};