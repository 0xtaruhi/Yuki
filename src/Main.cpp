#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Basic.hpp"
#include "MainScene.hpp"
#include "StartupScene.hpp"
#include "ResultScene.hpp"

using namespace yuki;

int main() {
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;

  sf::Music bgm;
  sf::Music main_bgm;
  bgm.setLoop(true);
  main_bgm.setLoop(true);

  sf::RenderWindow window(sf::VideoMode(kWindowWidth, kWindowHeight), "Yuki");
  window.setFramerateLimit(60);

  auto image = sf::Image{};
  if (!image.loadFromFile("assets/image/yuki_icon.png")) {
    std::cout << "Failed to load icon" << std::endl;
  }
  window.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());

  auto scene_ret = 0;
  {
    bgm.openFromFile("assets/music/bgm1.ogg");
    bgm.play();
    auto startup_scene = std::make_unique<StartupScene>(window);
    scene_ret = startup_scene->show();
    bgm.stop();
  }

  if (scene_ret == 1) {
    main_bgm.openFromFile("assets/music/main_bgm1.ogg");
    main_bgm.play();
    auto main_scene = std::make_unique<MainScene>(window);
    scene_ret = main_scene->show();
    main_bgm.stop();
  }

  auto result_scene = std::make_unique<ResultScene>(window, scene_ret == 1);
  result_scene->show();

  return 0;
}
