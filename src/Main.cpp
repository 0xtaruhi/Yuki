#include <SFML/Graphics.hpp>

#include "Basic.hpp"
#include "MainScene.hpp"
#include "StartupScene.hpp"

using namespace yuki;

int main() {
  sf::RenderWindow window(sf::VideoMode(kWindowWidth, kWindowHeight), "Yuki");
  window.setFramerateLimit(60);

  auto image = sf::Image {};
  if(!image.loadFromFile("assets/image/yuki_icon.png")) {
    std::cout << "Failed to load icon" << std::endl;
  }
  window.setIcon(image.getSize().x,image.getSize().y,image.getPixelsPtr());

  auto scene_ret = 0;
  {
    auto startup_scene = std::make_unique<StartupScene>(window);
    scene_ret = startup_scene->show();
  }

  if (scene_ret == 1) {
    auto main_scene = std::make_unique<MainScene>(window);
    main_scene->show();
  }

  return 0;
}
