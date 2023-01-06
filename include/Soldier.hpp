#ifndef SOLDIER_HPP
#define SOLDIER_HPP

#include <SFML/Graphics.hpp>
#include <string>

#include "Basic.hpp"
#include "Elementum.hpp"
#include "FloatingBubble.hpp"
#include "InteractiveObject.hpp"
#include "StatusBar.hpp"
#include "YukiScene.hpp"

namespace yuki {
class Soldier : public sf::Drawable, public Focusable {
 public:
  Soldier(Camp camp = Camp::Own) : camp_(camp) {
    health_bar_ = getHealthBar();
    setOrigin({sprite_.getLocalBounds().width / 2.f,
               sprite_.getLocalBounds().height});
  }
  Soldier(const sf::Vector2f& position, Camp camp = Camp::Own) : Soldier(camp) {
    setPosition(position);
  }

  virtual ~Soldier() {}

  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override;

  // Focusable object
  FOCUSABLE_OBJECT(Soldier)

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
  void setOrigin(const sf::Vector2f& origin) { sprite_.setOrigin(origin); }
  const auto getOrigin() const { return sprite_.getOrigin(); }

  auto getSize() const {
    return sf::Vector2f(sprite_.getGlobalBounds().width,
                        sprite_.getGlobalBounds().height);
  }

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

  void setFloatingBubbleVisible(const bool visible) {
    floating_bubble_visible_ = visible;
  }
  constexpr auto isFloatingBubbleVisible() const {
    return floating_bubble_visible_;
  }

  auto& getFloatingBubble() { return floating_bubble_; }
  const auto& getFloatingBubble() const { return floating_bubble_; }

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

  FloatingBubble floating_bubble_;
  bool floating_bubble_visible_;

  sf::Clock clock_;
};

class NormalSoldier : public Soldier {
 public:
  NormalSoldier(Camp camp = Camp::Own);
  NormalSoldier(const sf::Vector2f& position, Camp camp = Camp::Own);

  virtual ~NormalSoldier() {}

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

class SeniorSoldier : public Soldier {
 public:
  SeniorSoldier(Camp camp = Camp::Own);
  SeniorSoldier(const sf::Vector2f& position, Camp camp = Camp::Own);

  virtual ~SeniorSoldier() {}

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  
  virtual void update() override;

private:
  sf::Texture all_textures_;
  sf::Vector2f place_offset_ = {0,0};

  int current_frame_;
  int previous_frame_;
  int max_frame_;

  FloatingBubble floating_bubble_;

};
std::unique_ptr<NormalSoldier> getNormalSoldier(Camp camp = Camp::Own);
std::unique_ptr<Soldier> getSoldier(const std::string& name,
                                    Camp camp = Camp::Own);

}  // namespace yuki

#endif
