#include "MilitaryBase.hpp"

#include "Tile.hpp"

using namespace yuki;
using namespace sf;

MilitaryBase::MilitaryBase()
    : floating_bubble_(FloatingBubble(Vector2f(this->getPosition()), 120, 5, 15,
                                      Direction::Up)) {
  initTexture();
  bindHover([this](sf::Event) { floating_bubble_visible_ = true; });
  bindLeave([this](sf::Event) { floating_bubble_visible_ = false; });

  auto fb_pos = Vector2f({getPosition().x + getGlobalBounds().width / 2,
                          getPosition().y + getGlobalBounds().height / 2});
  floating_bubble_.setPosition(fb_pos);
}

void MilitaryBase::initTexture() { updateTexture(); }

void MilitaryBase::updateTexture() {
  auto getTextureLoc = [](int level) {
    return "assets/map/" +
           getTileSrcName(TileInfo(TileCategory::Building, level)) + ".png";
  };
  if (level >= 1 && level <= 3) {
    texture_.loadFromFile(getTextureLoc(level));
  } else {
    texture_.loadFromFile(getTextureLoc(1));
  }

  setTexture(texture_);
  setScale({0.4, 0.4});
}

bool MilitaryBase::inRange(const sf::Vector2f& position) const {
  return getGlobalBounds().contains(position);
}

void MilitaryBase::draw(sf::RenderTarget& target,
                        sf::RenderStates states) const {
  TouchableSprite::draw(target, states);
  if (floating_bubble_visible_) {
    floating_bubble_.draw(target, states);
  }
}
