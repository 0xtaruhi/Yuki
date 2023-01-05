#ifndef SODIER_HPP
#define SODIER_HPP

#include <SFML/Graphics.hpp>

#include "Basic.hpp"
#include "Elementum.hpp"
#include "InteractiveObject.hpp"
#include "StatusBar.hpp"
#include "YukiScene.hpp"
#include "FloatingBubble.hpp"

namespace yuki {
class Sodier : public sf::Drawable, public Focusable {
 public:
  Sodier(Camp camp = Camp::Own) : camp_(camp) { health_bar_ = getHealthBar(); }
  Sodier(const sf::Vector2f& position, Camp camp = Camp::Own) : Sodier(camp) {
    setPosition(position);
  }

  virtual ~Sodier() {}

  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override = 0;

  // Focusable object
  FOCUSABLE_OBJECT(Sodier)

  virtual void update();

  // Action
  void setDirection(const Direction direction) { direction_ = direction; }
  constexpr auto getDirection() const { return direction_; }
  void setSpeed(const float speed) { speed_ = speed; }
  constexpr auto getSpeed() const { return speed_; }
  void setMoving(const bool moving) { is_moving_ = moving; }
  constexpr auto isMoving() const { return is_moving_; }
  void setPosition(const sf::Vector2f& position) {
    sprite_.setPosition(position);
    health_bar_.setPosition(
        {position.x + health_bar_offset_.x, position.y + health_bar_offset_.y});
  }
  const auto getPosition() const { return sprite_.getPosition(); }

  void setMaxHealth(const float max_health) {
    health_bar_.setMaxValue(max_health);
  }
  constexpr auto getMaxHealth() const { return health_bar_.getMaxValue(); }
  void setHealth(const float current_health) {
    health_bar_.setValue(current_health);
  }
  constexpr auto getHealth() const { return health_bar_.getCurrentValue(); }
  void increaseHealth(const float health) { health_bar_.increase(health); }
  void decreaseHealth(const float health) { health_bar_.decrease(health); }

  void setColor(const sf::Color& color) { sprite_.setColor(color); }

  void setPlaceOffset(const sf::Vector2f& offset) { place_offset_ = offset; }
  virtual const sf::Vector2f& getPlaceOffset() const { return place_offset_; }

 protected:
  // sf::Sprite sprite_;
  sf::Sprite sprite_;
  sf::Vector2f health_bar_offset_ = {0, -15};
  Camp camp_;

  sf::Vector2f place_offset_ = {0, 0};

  bool is_moving_ = false;
  Direction direction_ = Direction::Down;
  float speed_ = 0.0f;
  Elementum elementum_ = Elementum(yuki::ElementumType::None, 0.0f);
  StatusBar health_bar_;

  sf::Clock clock_;
};

class NormalSodier : public Sodier {
 public:
  NormalSodier(Camp camp = Camp::Own);
  NormalSodier(const sf::Vector2f& position, Camp camp = Camp::Own);

  virtual ~NormalSodier() {}

  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override;

  virtual void update() override;

 private:
  sf::Texture all_textures_;

  sf::Vector2f place_offset_ = {0, 0};

  int current_frame_;
  int previous_frame_;
  int max_frame_;

  FloatingBubble floating_bubble_;
};

}  // namespace yuki

#endif
