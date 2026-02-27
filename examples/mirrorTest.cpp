//
// Created by Evan on 2/22/2026.
//
#include "../graphicsLib/Scene.h"
#include "../graphicsLib/BlinnPhongShader.h"
#include "../graphicsLib/Framebuffer.h"
#include "../graphicsLib/LambertianShader.h"
#include "../graphicsLib/MirrorShader.h"
#include "../graphicsLib/Sphere.h"
#include "../graphicsLib/Triangle.h"
#include "../graphicsLib/PointLight.h"

int main() {
  Scene world(100,100);

  auto lambertian  = std::make_shared<LambertianShader>(world);
  auto blinnPhong = std::make_shared<BlinnPhongShader>(world);
  auto mirror = std::make_shared<MirrorShader>(world);

  world.addLight(std::make_shared<PointLight>(vec3(-5,-5,5), color(255,255,255), 0.5));
  world.addLight(std::make_shared<PointLight>(vec3(5,-5,5), color(255,255,255), 0.5));

  //Ground Object
  world.addShape(std::make_shared<Sphere>(
    point3(0, 50, -20),
    50.0f,
    lambertian,
    color(71, 40, 20)
    ));

  //Right Sphere
  world.addShape(std::make_shared<Sphere>(
    point3(2, 0, -10),
      2.0f,
      lambertian,
      color(255, 0, 0)
  ));

  //Left Sphere
  world.addShape(std::make_shared<Sphere>(
    point3(-1, 0, -10),
    1.0f,
    mirror,
    color(100, 100, 100)));

  //Middle Sphere
  world.addShape(std::make_shared<Sphere>(
    point3(0, 1, -8),
    0.75f,
    blinnPhong,
    color(0, 0, 255)));

  world.generateScene();

  world.exportScene("MirrorTest.png");

  return 0;

};