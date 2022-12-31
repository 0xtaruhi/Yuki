#ifndef SODIER_HPP
#define SODIER_HPP

#include <SFML/Graphics.hpp>

#include "Basic.hpp"
#include "Elementum.hpp"
#include "StatusBar.hpp"

namespace yuki {
class Sodier : public sf::Drawable {
 public:
  Sodier() {
    is_moving_ = false;
    direction_ = Direction::Down;
    speed_ = 0.0f;
    health_bar_ = getHealthBar();
  }
  virtual ~Sodier() {}

  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const = 0;

  virtual void update();

  void setDirection(const Direction direction) { direction_ = direction; }
  constexpr auto getDirection() const { return direction_; }
  void setSpeed(const float speed) { speed_ = speed; }
  constexpr auto getSpeed() const { return speed_; }
  void setMoving(const bool moving) { is_moving_ = moving; }
  constexpr auto isMoving() const { return is_moving_; }

 protected:
  sf::Sprite sprite_;

  bool is_moving_;
  Direction direction_;
  float speed_;
  Elementum elementum_;
  StatusBar health_bar_;

  sf::Clock clock_;
};

class NormalSodier : public Sodier {
 public:
  NormalSodier();
  virtual ~NormalSodier() {}

  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override;

  virtual void update() override;

 private:
  sf::Texture all_textures_;

  int current_frame_;
  int previous_frame_;
  int max_frame_;
};
}  // namespace yuki

#endif
