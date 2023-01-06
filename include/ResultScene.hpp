#ifndef RESULT_SCENE_HPP
#define RESULT_SCENE_HPP

#include <SFML/Graphics.hpp>

#include "YukiScene.hpp"

namespace yuki {

class ResultScene : public YukiScene {
  YUKI_SCENE

 public:
  ResultScene(sf::RenderWindow& window, bool own_win = true);
  virtual ~ResultScene();

 private:
  sf::Text result_text_;
  bool own_win_;

  sf::Sprite background_;
  sf::Texture background_texture_;

  void initUi();
};

}  // namespace yuki

#endif  // RESULT_SCENE_HPP
