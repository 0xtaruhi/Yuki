#ifndef MAIN_SCENE_HPP
#define MAIN_SCENE_HPP

#include <queue>

#include "Config.hpp"
#include "InfoBar.hpp"
#include "Map.hpp"
#include "MilitaryBase.hpp"
#include "Soldier.hpp"
#include "YukiScene.hpp"

namespace yuki {

class MainScene : public YukiScene {
  YUKI_SCENE

 public:
  enum class Message {
    GenerateOwnSoldier,
    OwnBaseLevelUp,
    FocusedObjectChanged,
  };

  enum class ObjectType {
    OwnSoldier,
    EnemySoldier,
    OwnBase,
    EnemyBase,
  };

 public:
  MainScene(sf::RenderWindow& window);
  virtual ~MainScene() {}

  void generateSoldier();
  void sendMessage(const Message& message) { message_queue_.push(message); }

 private:
  const static sf::Vector2i kOwnSoldierBirthCoordinate;
  const static sf::Vector2i kEnemySoldierBirthCoordinate;

  Map map_;
  MilitaryBase own_base_;
  MilitaryBase enemy_base_;
  InfoBar info_bar_;

  int money_;

  std::vector<std::shared_ptr<Soldier>> soldiers_;
  std::vector<std::shared_ptr<Soldier>> enemies_;

  std::shared_ptr<Focusable> focused_object_;
  ObjectType focused_object_type_;

  void initUi();
  void initMap();
  void initBuildings();
  void initInfoHint();

  void setMoney(int money);

  std::shared_ptr<yuki::Soldier> getDefaultSoldier(Camp camp = Camp::Own);

  void eraseDeadSoldier();

  sf::Vector2f coordinateToPixel(const sf::Vector2i& coordinate);
  sf::Vector2i pixelToCoordinate(const sf::Vector2f& pixel_position);

  std::queue<Message> message_queue_;

#ifdef YUKI_DEBUG
  sf::Text debug_text_;
#endif
};

}  // namespace yuki

#endif  // MAIN_SCENE_HPP
