#ifndef STARTUP_SCENE_HPP
#define STARTUP_SCENE_HPP

#include <SFML/Graphics.hpp>

#include "Menu.hpp"
#include "YukiScene.hpp"

namespace yuki {

class StartupScene : public YukiScene {
 YUKI_SCENE
 public:
  StartupScene(sf::RenderWindow& window);
  virtual ~StartupScene() {}

 private:
  Button btn_start_;
  sf::Sprite background_;
  sf::Texture background_texture_;

  void initUi();
};

}  // namespace yuki

#endif  // STARTUP_SCENE_HPP
