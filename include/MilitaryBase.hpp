#ifndef MILITARY_BASE_HPP
#define MILITARY_BASE_HPP

#include "FloatingBubble.hpp"
#include "TouchableSprite.hpp"

namespace yuki {

class MilitaryBase : public TouchableSprite {
  TOUCHABLE_OBJECT(MilitaryBase)

 public:
  MilitaryBase();
  virtual ~MilitaryBase() {}

  void levelUp() {
    level++;
    updateTexture();
  }
  void levelDown() {
    level--;
    updateTexture();
  }

  void setPosition(const sf::Vector2f& position) {
    TouchableSprite::setPosition(position);
    floating_bubble_.setPosition({position.x + getGlobalBounds().width / 2.f,
                                  position.y + getGlobalBounds().height / 2.f});
  }

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  void setFloatingBubbleVisible(bool visible) {
    floating_bubble_visible_ = visible;
  }

  int getFloatingBubbleIndexByPosition(const sf::Vector2f& position) {
    if (!floating_bubble_visible_) {
      return -1;
    }
    return floating_bubble_.getIndexByPosition(position);
  }

 private:
  int level = 1;
  sf::Texture texture_;
  FloatingBubble floating_bubble_;
  bool floating_bubble_visible_ = false;

  void initTexture();
  void updateTexture();
};

}  // namespace yuki

#endif
