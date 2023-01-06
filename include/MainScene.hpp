#ifndef MAIN_SCENE_HPP
#define MAIN_SCENE_HPP

#include <queue>
#include <vector>
#include <array>

#include "Config.hpp"
#include "InfoBar.hpp"
#include "Map.hpp"
#include "MilitaryBase.hpp"
#include "Skill.hpp"
#include "Soldier.hpp"
#include "YukiScene.hpp"
#include "ElementumPanel.hpp"

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
  // const static sf::Vector2i kOwnBaseCoordinate;
  const static sf::Vector2i kEnemyBaseCoordinate;

  using GranuleAttackWithSenderInfo = std::pair<Soldier*, GranuleAttack>;

  Map map_;
  std::unique_ptr<MilitaryBase> own_base_;
  std::unique_ptr<MilitaryBase> enemy_base_;
  std::unique_ptr<InfoBar> info_bar_;
  std::unique_ptr<SkillBar<3>> skill_bar_;

  int money_;

  std::vector<std::unique_ptr<Soldier>> soldiers_;
  std::vector<std::unique_ptr<Soldier>> enemies_;

  Focusable* focused_object_;
  ObjectType focused_object_type_;

  void initUi();
  void initMap();
  void initBuildings();
  void initInfoHint();

  void setMoney(int money);

  // std::shared_ptr<yuki::Soldier> getDefaultSoldier(Camp camp = Camp::Own);
  std::unique_ptr<yuki::Soldier> generateSoldier(const std::string& name,
                                                 Camp camp = Camp::Own);

  void eraseDeadSoldier();
  void sodierAdjustDirection();
  void updateAttacks();

  sf::Vector2f coordinateToPixel(const sf::Vector2i& coordinate);
  sf::Vector2i pixelToCoordinate(const sf::Vector2f& pixel_position);

  std::vector<GranuleAttackWithSenderInfo> granule_attacks_;
  void registerGranuleAttack(Soldier* soldier, const GranuleAttack& attack) {
    granule_attacks_.push_back(std::make_pair(soldier, attack));
  }

  // Elementum Panel
  std::array<ElementumPanel, 3> elementum_panels_;
  ElementumType own_current_elementum_type_;
  void initElementumPanel();
  void setOwnCurrentElementumType(ElementumType type);

  // Attack Button
  void initButton();
  TouchableObject<sf::CircleShape> attack_button_;

  std::queue<Message> message_queue_;

#ifdef YUKI_DEBUG
  sf::Text debug_text_;
#endif
};

}  // namespace yuki

#endif  // MAIN_SCENE_HPP
