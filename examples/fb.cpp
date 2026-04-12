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

void printUsage(const char * programName) {
  std::cout << "Usage: " << programName << " [width] [height] [output filename]\n"
            << "  width    - Image width in pixels  (default: 250)\n"
            << "  height   - Image height in pixels (default: 250)\n"
            << "  filename - Output PNG filename    (default: MirrorTest.png)\n"
            << "Example: " << programName << " 1920 1080 output.png\n";
}

int main(int argc, char* argv[]) {
  int width = 250;
  int height = 250;
  std::string filename = "Default.png";

  //parse command line arguments
  if (argc > 1) {
    std::string firstArg(argv[1]);
    if (firstArg == "--help" || firstArg == "-h") {
      printUsage(argv[0]);
      return 0;
    }

    try {
      if (argc >= 2) {
        width = std::stoi(argv[1]);
        if (width <= 0) throw std::invalid_argument("width must be greater than zero");
      }
      if (argc >= 3) {
        height = std::stoi(argv[2]);
        if (height <= 0) throw std::invalid_argument("height must be greater than zero");
      }
      if (argc >= 4) {
        filename = argv[3];
        //Appending .png extension
        if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".png") {
          filename += ".png";
        }
      }
    } catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << std::endl;
      printUsage(argv[0]);
      return 1;
    }
  }

  Scene world(width, height);

  PerspectiveCamera cam = PerspectiveCamera(width, height);
  cam.moveTo(vec3(0, 1, 0));
  cam.lookAt(vec3(0, 1, -8));
  world.addCamera(std::make_shared<PerspectiveCamera>(cam));

  auto lambertian  = std::make_shared<LambertianShader>(world);
  auto blinnPhong = std::make_shared<BlinnPhongShader>(world);
  auto mirror = std::make_shared<MirrorShader>(world);

  world.addLight(std::make_shared<PointLight>(vec3(-5,-15,-15), color(255,255,255), 0.5));
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
  world.exportScene(filename);
  return 0;

};