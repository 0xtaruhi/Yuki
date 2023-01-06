#ifndef MILITARY_BASE_HPP
#define MILITARY_BASE_HPP

#include "FloatingBubble.hpp"
#include "StatusBar.hpp"
#include "TouchableSprite.hpp"


namespace yuki {

class MilitaryBase : public TouchableSprite {
  TOUCHABLE_OBJECT(MilitaryBase)

  static constexpr float health_bar_offset_y = -10.f;

 public:
  MilitaryBase(Camp camp = Camp::Own);
  virtual ~MilitaryBase() {}

  void levelUp() {
    level++;
    updateTexture();
  }
  void levelDown() {
    level--;
    updateTexture();
  }

  void setCurrentHealth(int health) {
    health_bar_.setValue(health);
  }
  void setMaxHealth(int health) {
    health_bar_.setMaxValue(health);
  }
  constexpr auto getCurrentHealth() const {
    return health_bar_.getValue();
  }
  constexpr auto getMaxHealth() const {
    return health_bar_.getMaxValue();
  }

  void setPosition(const sf::Vector2f& position) {
    TouchableSprite::setPosition(position);
    floating_bubble_.setPosition({position.x + getGlobalBounds().width / 2.f,
                                  position.y + getGlobalBounds().height / 2.f});
    health_bar_.setPosition({position.x, position.y + health_bar_offset_y});
  }

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  void setFloatingBubbleVisible(bool visible) {
    floating_bubble_visible_ = visible;
  }
  constexpr auto getFloatingBubbleVisible() const {
    return floating_bubble_visible_;
  }
  // void setHealthBarVisible(bool visible) { health_bar_visible_ = visible; }
  // constexpr auto getHealthBarVisible() const { return health_bar_visible_; }

  int getFloatingBubbleIndexByPosition(const sf::Vector2f& position) {
    if (!floating_bubble_visible_) {
      return -1;
    }
    return floating_bubble_.getIndexByPosition(position);
  }

  auto& getFloatingBubble() { return floating_bubble_; }
  const auto& getFloatingBubble() const { return floating_bubble_; }

 private:
  Camp camp_;
  int level = 1;
  sf::Texture texture_;
  FloatingBubble floating_bubble_;
  bool floating_bubble_visible_ = false;
  StatusBar health_bar_;
  // bool health_bar_visible_ = false;

  void initTexture();
  void updateTexture();
};

}  // namespace yuki

#endif
