#include "MilitaryBase.hpp"

#include "Tile.hpp"

using namespace yuki;
using namespace sf;

MilitaryBase::MilitaryBase(Camp camp)
    : camp_(camp),
      floating_bubble_(FloatingBubble(Vector2f(this->getPosition()), 110, 4, 22,
                                      Direction::Up)) {
  initTexture();
  bindHover([this](sf::Event) {
    floating_bubble_visible_ = true;
    health_bar_visible_ = true;
  });
  bindLeave([this](sf::Event) {
    floating_bubble_visible_ = false;
    health_bar_visible_ = false;
  });

  auto fb_pos = Vector2f({getPosition().x + getGlobalBounds().width / 2,
                          getPosition().y + getGlobalBounds().height / 2});
  floating_bubble_.setPosition(fb_pos);

  health_bar_ = getHealthBar(
      {
          this->getGlobalBounds().width,
          20.f,
      },
      camp);
  health_bar_.setPosition(
      {this->getPosition().x, this->getPosition().y - 30.f});
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
  // return getGlobalBounds().contains(position);
  return getGlobalBounds().contains(position);
}

void MilitaryBase::draw(sf::RenderTarget& target,
                        sf::RenderStates states) const {
  TouchableSprite::draw(target, states);
  if (health_bar_visible_) {
    target.draw(health_bar_, states);
  }
  if (floating_bubble_visible_) {
    floating_bubble_.draw(target, states);
  }
}
