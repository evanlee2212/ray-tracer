//
// Created by Evan on 3/1/2026.
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
  int width = 100;
  int height = 100;
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

  //World Settings:
  //world.setBG(color(200, 0, 0 ));

  //Camera
  world.moveTo(point3(0, 0, -5));
  //world.lookAt(point3(2,0,-6));

  //Shaders
  auto lambertian  = std::make_shared<LambertianShader>(world);
  auto blinnPhong = std::make_shared<BlinnPhongShader>(world);
  auto mirror = std::make_shared<MirrorShader>(world);

  //Lights
  world.addLight(std::make_shared<PointLight>(vec3(5,-15,0), color(255,255, 255), 1));

  //Ground
  world.addShape(std::make_shared<Triangle>(
    point3(0, 1, -6),
    point3(100, 1, 50),
    point3(-100, 1, 50),
    lambertian,
    color(75,200,125)));

  //Middle Sphere
  world.addShape(std::make_shared<Sphere>(
    point3(0, 0.5, 5),
    0.5f,
    mirror,
    color(255, 255, 255)));

  //Right Sphere
  world.addShape(std::make_shared<Sphere>(
    point3(1, 0.5, 5),
    0.5f,
    lambertian,
    color(0, 0, 255)));

  //Right Sphere
  world.addShape(std::make_shared<Sphere>(
    point3(-1, 0.5, 5),
    0.5f,
    blinnPhong,
    color(255, 0, 0)));



  world.generateScene();

  world.exportScene(filename);

  return 0;

};