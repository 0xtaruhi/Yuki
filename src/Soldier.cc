#include "Soldier.h"

#include "YukiBasic.h"

using namespace yuki;

AbstractSoldier::AbstractSoldier() {}

AbstractSoldier::AbstractSoldier(WindowHandler parent_window)
    : parent_window_(parent_window) {}

AbstractSoldier::~AbstractSoldier() {}

void AbstractSoldier::walk(const unsigned int distance) {
  switch (direction_) {
    case (Moving::Left):
      position_.x -= distance;
      break;
    case (Moving::Right):
      position_.x += distance;
      break;
    case (Moving::Up):
      position_.y -= distance;
      break;
    case (Moving::Down):
      position_.y += distance;
      break;
    default:
      break;
  };
}

void AbstractSoldier::walk(const sf::Vector2u& shift) {
  if (direction_ == Moving::Other) {
    position_ += shift;
  }
}

void AbstractSoldier::update() { this->walk(speed_); }

NormalSoldier::NormalSoldier() : AbstractSoldier() {
  soldier_.setSize(sf::Vector2f(cellLength, cellLength));
  soldier_.setFillColor(sf::Color::Red);
}

NormalSoldier::NormalSoldier(WindowHandler parent_window,
                             const sf::Vector2u& position)
    : AbstractSoldier(parent_window) {
  soldier_.setSize(sf::Vector2f(cellLength, cellLength));
  soldier_.setFillColor(sf::Color::Red);
  this->setPosition(position);
}

NormalSoldier::NormalSoldier(WindowHandler parent_window)
    : NormalSoldier(parent_window, sf::Vector2u(0, 0)) {}

NormalSoldier::~NormalSoldier() {}

void NormalSoldier::draw() {
  soldier_.setPosition(sf::Vector2f(position_.x, position_.y));
  parent_window_->draw(soldier_);
}
