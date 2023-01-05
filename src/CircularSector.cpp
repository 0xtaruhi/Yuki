#include "CircularSector.hpp"

#include <cmath>

using namespace sf;
using namespace yuki;

constexpr auto kPi = 3.14159265358979323846f;

Vector2f CircularSector::getPoint(std::size_t index) const {
  if (index == 0) return center_;

  auto angle = start_angle_ + index - 1;
  auto angle_rad = angle * kPi / 180.f;

  auto x = center_.x + radius_ * std::sin(angle_rad);
  auto y = center_.y + radius_ * std::cos(angle_rad);

  return {x, y};
}
