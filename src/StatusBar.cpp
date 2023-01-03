#include "StatusBar.hpp"

using namespace yuki;
using namespace sf;

void StatusBar::setPosition(const sf::Vector2f& position) {
  background_.setPosition(position);
  filled_.setPosition(position);
}

void StatusBar::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  if (!visible_) return;
  target.draw(background_, states);
  target.draw(filled_, states);
}

void StatusBar::setFilledPercentage(float percentage) {
  filled_.setSize({size_.x * percentage, size_.y});
}

void StatusBar::increase(const float value) {
  current_value_ += value;
  adjustIfOverflow();
  updateFilledPercentage();
}

void StatusBar::decrease(const float value) {
  current_value_ -= value;
  adjustIfOverflow();
  updateFilledPercentage();
}

void StatusBar::adjustIfOverflow() {
  if (current_value_ > max_value_) current_value_ = max_value_;
  if (current_value_ < 0) current_value_ = 0;
}
