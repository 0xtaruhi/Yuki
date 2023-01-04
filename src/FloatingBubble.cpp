#include "FloatingBubble.hpp"

#include <cmath>

using namespace sf;
using namespace yuki;

constexpr float kPi = 3.14159265358979323846f;

bool BubbleItem::inRange(const sf::Vector2f& position) const {
  return getGlobalBounds().contains(position);
}

// bool FloatingBubble::inRange(const sf::Vector2f& position) const {
//   for (const auto& item : bubbles_) {
//     if (item->inRange(position)) {
//       return true;
//     }
//   }
//   return false;
// }

Vector2f FloatingBubble::getBubblePosition(const int index) {
  Vector2f position;
  bool clockwise = false;
  float base_degree = 0.f;

  auto degreeToRadian = [](const float degree) { return degree * kPi / 180.f; };

  switch (direction_) {
    case Direction::Up:
      clockwise = true;
      base_degree = 135.f;
      break;
    case Direction::Down:
      clockwise = false;
      base_degree = -135.f;
      break;
    case Direction::Right:
      clockwise = true;
      base_degree = 45.f;
      break;
    case Direction::Left:
      clockwise = false;
      base_degree = 135.f;
      break;
    default:
      clockwise = false;
      base_degree = 0.f;
  }

  float direction_gap_degree = 5.f;
  float total_distribution_degree = 90.f - 2 * direction_gap_degree;

  // base_degree += clockwise ? -direction_gap_degree : direction_gap_degree;
  float item_gap_degree = total_distribution_degree / (bubble_num_ - 1);

  float degree;
  if (clockwise) {
    degree = base_degree - direction_gap_degree - item_gap_degree * index;
  } else {
    degree = base_degree + direction_gap_degree + item_gap_degree * index;
  }
  auto radian = degreeToRadian(degree);

  position.x = position_.x + distribute_radius_ * std::cos(radian);
  position.y = position_.y - distribute_radius_ * std::sin(radian);

  return position;
}

int FloatingBubble::getIndexByPosition(const sf::Vector2f& position) const {
  for (int i = 0; i < bubble_num_; ++i) {
    if (bubbles_[i]->inRange(position)) {
      return i;
    }
  }
  return -1;
}

void FloatingBubble::draw(RenderTarget& target, RenderStates states) const {
  for (const auto& bubble : bubbles_) {
    target.draw(*bubble, states);
  }
}

void FloatingBubble::updateBubblesPosition() {
  for (int i = 0; i < bubble_num_; ++i) {
    bubbles_[i]->setPosition(getBubblePosition(i));
  }
}
