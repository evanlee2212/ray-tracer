#include <iostream>

#include "../graphicsLib/Scene.h"
#include "SceneLoader.h"
#include "SceneParser_JSON.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <scene_file.json>" << std::endl;
    return 1;
  }


  Scene scene;

  std::shared_ptr<ISceneLoader> loader = std::make_shared<SceneLoader>(scene);
  SceneParser_JSON parser(loader);

  std::string filename = argv[1];
  parser.parseFileData(filename);

  scene.generateScene();
  scene.exportScene(filename);
}