#include "Attack.hpp"

using namespace yuki;
using namespace sf;

void GranuleAttack::draw(RenderTarget& target, RenderStates states) const {
  target.draw(granule_, states);
}

void GranuleAttack::update() {
  switch (direction_) {
    case Direction::Up:
      granule_.move(0, -speed_);
      break;
    case Direction::Down:
      granule_.move(0, speed_);
      break;
    case Direction::Left:
      granule_.move(-speed_, 0);
      break;
    case Direction::Right:
      granule_.move(speed_, 0);
      break;
    default:
      break;
  }
}
