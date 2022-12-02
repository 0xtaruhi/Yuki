#ifndef SOLDIER_H
#define SOLDIER_H

#include <SFML/Graphics.hpp>

#include "YukiBasic.h"

namespace yuki {

class AbstractSoldier {
 public:
  AbstractSoldier();
  AbstractSoldier(WindowHandler parent_window);
  virtual ~AbstractSoldier();

  // Graphics
  virtual void draw() = 0;

  constexpr auto getHp() const { return hp_; }
  constexpr auto getMaxHp() const { return max_hp_; }
  void setHp(int hp) { hp_ = hp; }
  void setMaxHp(int max_hp) { max_hp_ = max_hp; }

  void setPosition(const sf::Vector2u& new_pos) { position_ = new_pos; }
  const auto getPosition() const { return position_; }

  void setDirection(const Moving direction) { direction_ = direction; }
  const auto getDirection() const { return direction_; }

  void setSpeed(const int speed) { speed_ = speed; }
  constexpr auto getSpeed() const { return speed_; }

  void walk(const unsigned int distance);
  void walk(const sf::Vector2u& shift);
  virtual void update();

 protected:
  WindowHandler parent_window_;
  sf::Vector2u position_;
  Moving direction_;
  int speed_;

  int hp_;
  int max_hp_;
};

class NormalSoldier : public AbstractSoldier {
 public:
  NormalSoldier();
  NormalSoldier(WindowHandler parent_window, const sf::Vector2u& position);
  NormalSoldier(WindowHandler parent_window);
  virtual ~NormalSoldier();

  void draw() override;

 private:
  sf::RectangleShape soldier_;
};

};  // namespace yuki

#endif
