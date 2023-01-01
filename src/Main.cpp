#include <SFML/Graphics.hpp>

#include "StartupScene.hpp"

using namespace yuki;

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 600), "Yuki");
  
  auto startup_scene = std::make_unique<StartupScene>(window);
  startup_scene->show();

  return 0;
}
