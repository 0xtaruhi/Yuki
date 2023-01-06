#ifndef MAIN_SCENE_HPP
#define MAIN_SCENE_HPP

#include <array>
#include <queue>
#include <vector>

#include "Config.hpp"
#include "ElementumPanel.hpp"
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
    GenerateEnemySoldier,
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

  // void generateSoldier();
  void sendMessage(const Message& message) { message_queue_.push(message); }

 private:
  const static sf::Vector2i kOwnSoldierBirthCoordinate;
  const static sf::Vector2i kEnemySoldierBirthCoordinate;
  const static sf::Vector2i kOwnBaseCoordinate;
  const static sf::Vector2i kEnemyBaseCoordinate;

  using GranuleAttackWithSenderInfo = std::pair<Soldier*, GranuleAttack>;

  Map map_;
  std::unique_ptr<MilitaryBase> own_base_;
  std::unique_ptr<MilitaryBase> enemy_base_;
  std::unique_ptr<InfoBar> info_bar_;
  std::unique_ptr<SkillBar<3>> skill_bar_;

  int own_money_;
  int enemy_money_;

  void increaseOwnMoney(int money) { increaseMoney(money, Camp::Own); }
  void decreaseOwnMoney(int money) { decreaseMoney(money, Camp::Own); }
  void increaseEnemyMoney(int money) { increaseMoney(money, Camp::Enemy); }
  void decreaseEnemyMoney(int money) { decreaseMoney(money, Camp::Enemy); }

  std::vector<std::unique_ptr<Soldier>> soldiers_;
  std::vector<std::unique_ptr<Soldier>> enemies_;

  Focusable* focused_object_;
  ObjectType focused_object_type_;

  void initUi();
  void initMap();
  void initBuildings();
  void initInfoHint();

  void setMoney(int money, Camp camp = Camp::Own);
  void increaseMoney(int money, Camp camp = Camp::Own);
  void decreaseMoney(int money, Camp camp = Camp::Own);

  // std::shared_ptr<yuki::Soldier> getDefaultSoldier(Camp camp = Camp::Own);
  std::unique_ptr<yuki::Soldier> generateSoldier(const std::string& name,
                                                 Camp camp = Camp::Own);

  void eraseDeadSoldier();
  // void sodierAdjustDirection();
  void soldierUpdatePosition();
  void updateAttacks();
  void enemyBaseMakeDecision();

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
  sf::Texture attack_button_texture_;

  std::queue<Message> message_queue_;

  void implMsgGenerateOwnSoldier();
  void implMsgGenerateEnemySoldier();

#ifdef YUKI_DEBUG
  sf::Text debug_text_;
#endif
};

}  // namespace yuki

#endif  // MAIN_SCENE_HPP
