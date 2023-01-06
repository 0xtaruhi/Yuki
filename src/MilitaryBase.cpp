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
    // health_bar_visible_ = true;
  });
  bindLeave([this](sf::Event) {
    floating_bubble_visible_ = false;
    // health_bar_visible_ = false;
  });

  auto fb_pos = Vector2f({getPosition().x + getGlobalBounds().width / 2,
                          getPosition().y + getGlobalBounds().height / 2});
  floating_bubble_.setPosition(fb_pos);

  // health_bar_ = getHealthBar(
  //     {
  //         this->getGlobalBounds().width,
  //         5.f,
  //     },
  //     camp);
  health_bar_ =
      StatusBar({this->getGlobalBounds().width, 3.f},
                {getPosition().x, getPosition().y + health_bar_offset_y});
  health_bar_.setBackgroudColor(Color::Black);
  health_bar_.setOutlineColor(Color::Black);
  health_bar_.setOutlineThickness(2.f);
  if (camp == Camp::Own) {
    health_bar_.setFilledColor(Color::Green);
  } else {
    health_bar_.setFilledColor(Color::Red);
  }
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
  target.draw(health_bar_, states);
  if (floating_bubble_visible_) {
    floating_bubble_.draw(target, states);
  }
}

bool MilitaryBase::canGenerateSoldier() {
  auto elapsed = generateSoldierClock_.getElapsedTime();
  if (elapsed >= seconds(1)) {
    generateSoldierClock_.restart();
    return true;
  }
  return false;
}
