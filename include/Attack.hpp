#ifndef ATTACK_HPP
#define ATTACK_HPP

#include <SFML/Graphics.hpp>
#include <queue>

#include "Basic.hpp"
#include "Elementum.hpp"

namespace yuki {

struct AttackInfo {
  ElementumType elementum_type;
  float elementum_level;
  float damage;

  AttackInfo(ElementumType elementum_type = ElementumType::None,
             float elementum_level = 0.0f, float damage = 0.0f)
      : elementum_type(elementum_type),
        elementum_level(elementum_level),
        damage(damage) {}
};

class BasicAttack {
 public:
  BasicAttack() = default;
  BasicAttack(ElementumType elementum_type, float elementum_level,
              float damage) {
    info_.elementum_type = elementum_type;
    info_.elementum_level = elementum_level;
    info_.damage = damage;
  }
  BasicAttack(const AttackInfo& info) : info_(info) {}

  virtual ~BasicAttack() {}

 private:
  AttackInfo info_;
};

class GranuleAttack : public BasicAttack, public sf::Drawable {
 public:
  GranuleAttack() = default;
  GranuleAttack(const sf::Vector2f& position, const AttackInfo& attck_info)
      : BasicAttack(attck_info) {
    granule_.setPosition(position);
  }

  virtual ~GranuleAttack() {}

  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override;

  virtual void update();

 protected:
  sf::Sprite granule_;
  float speed_;
  Direction direction_;
};

class GranuleAttackable {
 public:
  GranuleAttackable(float speed = 0.f, Direction direction = Direction::Up)
      : speed_(speed), direction_(direction) {}

  virtual ~GranuleAttackable() {}

  void addGranuleAttack(const sf::Vector2f& position,
                        const AttackInfo& attack_info) {
    granule_attacks_.push(GranuleAttack(position, attack_info));
  }
  void addGranuleAttack(const sf::Vector2f& position) {
    granule_attacks_.push(GranuleAttack(position, default_attack_info_));
  }

  void setDefaultAttackInfo(const AttackInfo& attack_info) {
    default_attack_info_ = attack_info;
  }

 private:
  std::queue<GranuleAttack> granule_attacks_;
  float speed_;
  Direction direction_;

  AttackInfo default_attack_info_;
};

}  // namespace yuki

#endif  // ATTACK_HPP
