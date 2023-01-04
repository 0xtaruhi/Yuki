#ifndef TOUCHABLE_SPRITE_HPP
#define TOUCHABLE_SPRITE_HPP

#include "InteractiveObject.hpp"

namespace yuki {

class TouchableSprite : public sf::Sprite, public yuki::Touchable {
  // TOUCHABLE_OBJECT(TouchableSprite)

 public:
  TouchableSprite() = default;
  TouchableSprite(const sf::Texture& texture) : sf::Sprite(texture) {}
  TouchableSprite(const sf::Texture& texture, const sf::IntRect& rect)
      : sf::Sprite(texture, rect) {}

  virtual ~TouchableSprite() {}

private:
  virtual bool inRange(const sf::Vector2f& position) const override {
    return getGlobalBounds().contains(position);
  }
};

} // namespace yuki

#endif
