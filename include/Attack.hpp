#ifndef ATTACK_HPP
#define ATTACK_HPP

#include <SFML/Graphics.hpp>

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

  virtual ~BasicAttack() = 0;

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

  virtual ~GranuleAttack() = 0;

  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override = 0;

 protected:
  sf::Sprite granule_;
};


}  // namespace yuki

#endif  // ATTACK_HPP

