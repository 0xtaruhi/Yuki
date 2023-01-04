#ifndef MAIN_SCENE_HPP
#define MAIN_SCENE_HPP

#include "Config.hpp"
#include "Map.hpp"
#include "Sodier.hpp"
#include "MilitaryBase.hpp"
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
  void initMap();

  void initBuildings();

  void generateSodier();

  std::shared_ptr<yuki::Sodier> getDefaultSodier();

  sf::Vector2f coordinateToPixel(const sf::Vector2i& coordinate);
  sf::Vector2i pixelToCoordinate(const sf::Vector2f& pixel_position);

  MilitaryBase own_base_;
  MilitaryBase enemy_base_;

#ifdef YUKI_DEBUG
  sf::Text debug_text_;
#endif
};

}  // namespace yuki

#endif  // MAIN_SCENE_HPP
