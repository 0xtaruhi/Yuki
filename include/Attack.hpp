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

  const auto& getAttackInfo() const { return info_; }

  const auto getElementum() const {
    return Elementum(info_.elementum_type, info_.elementum_level);
  }
  const auto getDamage() const { return info_.damage; }

  virtual ~BasicAttack() {}

 protected:
  AttackInfo info_;
};

class GranuleAttack : public BasicAttack, public sf::Drawable {
 public:
  GranuleAttack() { updateGranule(); }
  GranuleAttack(const sf::Vector2f& position, float speed, Direction direction,
                const AttackInfo& attck_info)
      : BasicAttack(attck_info) {
    speed_ = speed;
    direction_ = direction;
    granule_.setPosition(position);
    updateGranule();
  }

  virtual ~GranuleAttack() {}

  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override;

  virtual void update();

  auto getPosition() const { return granule_.getPosition(); }

 protected:
  sf::CircleShape granule_;
  float speed_;
  Direction direction_;

  void updateGranule();
};

}  // namespace yuki

#endif  // ATTACK_HPP
