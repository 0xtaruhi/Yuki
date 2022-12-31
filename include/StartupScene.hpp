#ifndef STARTUP_SCENE_HPP
#define STARTUP_SCENE_HPP

#include <SFML/Graphics.hpp>

#include "Menu.hpp"
#include "YukiScene.hpp"

namespace yuki {

class StartupScene : public YukiScene {
 public:
  StartupScene(sf::RenderWindow& window);

  void show() override;

  void draw() const override;
 private:
  Menu menu_;
};

}  // namespace yuki

#endif  // STARTUP_SCENE_HPP
