#ifndef MAIN_SCENE_HPP
#define MAIN_SCENE_HPP

#include "Map.hpp"
#include "Sodier.hpp"
#include "YukiScene.hpp"


namespace yuki {

class MainScene : public YukiScene {
  YUKI_SCENE

 public:
  MainScene(sf::RenderWindow& window);
  virtual ~MainScene() {}

 private:
  Map map_;

  std::vector<std::shared_ptr<Sodier>> soldiers_;
  std::vector<std::shared_ptr<Sodier>> enemies_;

  void initUi();
  void generateSodier();

  std::shared_ptr<yuki::Sodier> getDefaultSodier();
};

}  // namespace yuki

#endif  // MAIN_SCENE_HPP
