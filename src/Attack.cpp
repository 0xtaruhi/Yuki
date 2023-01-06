#include "Attack.hpp"

using namespace yuki;
using namespace sf;

void GranuleAttack::draw(RenderTarget& target, RenderStates states) const {
  target.draw(granule_, states);
}

void GranuleAttack::update() {
  switch (direction_) {
    case Direction::Up:
      granule_.setPosition({
          granule_.getPosition().x,
          granule_.getPosition().y - speed_,
      });
      break;
    case Direction::Down:
      granule_.setPosition({
          granule_.getPosition().x,
          granule_.getPosition().y + speed_,
      });
      break;
    case Direction::Left:
      granule_.setPosition({
          granule_.getPosition().x - speed_,
          granule_.getPosition().y,
      });
      break;
    case Direction::Right:
      granule_.setPosition({
          granule_.getPosition().x + speed_,
          granule_.getPosition().y,
      });
      break;
    default:
      break;
  }
}

void GranuleAttack::updateGranule() {
  switch (info_.elementum_type) {
    case ElementumType::Pyro:
      granule_.setFillColor(Color::Red);
      break;
    case ElementumType::Hydro:
      granule_.setFillColor(Color::Blue);
      break;
    case ElementumType::Cyro:
      granule_.setFillColor(Color::White);
      break;
    default:
      break;
  }

  granule_.setRadius(3.0f);
  granule_.setOutlineColor(Color::Black);
  granule_.setOutlineThickness(1.0f);
}
