#include "Skill.hpp"

using namespace sf;
using namespace yuki;



SkillIcon::SkillIcon(const sf::Vector2f& position, float radius,
                     float border_thick)
    : circular_sector_(position, radius + border_thick, 0, 360),
      background_(radius + border_thick),
      icon_(radius) {
  circular_sector_.setFillColor(Color::White);

  background_.setFillColor(Color::Black);
  background_.setOutlineColor(Color::White);
  background_.setOutlineThickness(2.f);

  icon_.setFillColor(Color::Transparent);

  background_.setPosition(position);
  icon_.setPosition(position);
}

void SkillIcon::setPosition(const sf::Vector2f& position) {
  background_.setPosition(position);
  icon_.setPosition(position);
  circular_sector_.setPosition(position);
}

bool SkillIcon::inRange(const sf::Vector2f& point) const {
  return background_.getGlobalBounds().contains(point);
}

void SkillIcon::draw(RenderTarget& target, RenderStates states) const {
  target.draw(background_, states);
  target.draw(circular_sector_, states);
  target.draw(icon_, states);
}

void SkillIcon::updateCircularSector() {
  circular_sector_.setEndAngle(360.f * current_value_ / max_value_);
}
