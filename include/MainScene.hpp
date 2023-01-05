#ifndef MAIN_SCENE_HPP
#define MAIN_SCENE_HPP

#include <queue>

#include "Config.hpp"
#include "InfoBar.hpp"
#include "Map.hpp"
#include "MilitaryBase.hpp"
#include "Skill.hpp"
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
  void sendMessage(const Message& message) { message_quene_.push(message); }

 private:
  const static sf::Vector2i kOwnSoldierBirthCoordinate;
  const static sf::Vector2i kEnemySoldierBirthCoordinate;
  // const static sf::Vector2i kOwnBaseCoordinate;
  const static sf::Vector2i kEnemyBaseCoordinate;

  Map map_;
  std::shared_ptr<MilitaryBase> own_base_;
  std::shared_ptr<MilitaryBase> enemy_base_;
  std::shared_ptr<InfoBar> info_bar_;
  std::shared_ptr<SkillBar<3>> skill_bar_;

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

  // std::shared_ptr<yuki::Soldier> getDefaultSoldier(Camp camp = Camp::Own);
  std::shared_ptr<yuki::Soldier> generateSoldier(const std::string& name,
                                                 Camp camp = Camp::Own);

  void eraseDeadSoldier();
  void sodierAdjustDirection();

  sf::Vector2f coordinateToPixel(const sf::Vector2i& coordinate);
  sf::Vector2i pixelToCoordinate(const sf::Vector2f& pixel_position);

  std::queue<Message> message_quene_;

#ifdef YUKI_DEBUG
  sf::Text debug_text_;
#endif
};

}  // namespace yuki

#endif  // MAIN_SCENE_HPP
