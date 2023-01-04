#ifndef FLOATING_BUBBLE_HPP
#define FLOATING_BUBBLE_HPP

#include <InteractiveObject.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

#include "Basic.hpp"

namespace yuki {

class BubbleItem : public sf::CircleShape, Touchable {
  friend class FloatingBubble;

 public:
  BubbleItem() = default;
  BubbleItem(float radius) : sf::CircleShape(radius) {}
  virtual ~BubbleItem() = default;
  TOUCHABLE_OBJECT(BubbleItem)
 private:
};

class FloatingBubble : public sf::Drawable {
 public:
  FloatingBubble(const sf::Vector2f& position, const float distribute_radius,
                 const int bubble_num, const float bubble_radius = 10.f,
                 const Direction direction = Direction::Down)
      : position_(position),
        distribute_radius_(distribute_radius),
        bubble_num_(bubble_num),
        bubble_radius_(bubble_radius),
        direction_(direction) {
    for (int i = 0; i < bubble_num_; ++i) {
      auto bubble = std::make_unique<BubbleItem>(bubble_radius);
      bubble->setFillColor(sf::Color::Transparent);
      bubble->setOutlineColor(sf::Color::Blue);
      bubble->setOutlineThickness(2.f);
      bubble->setPosition(getBubblePosition(i));
      bubbles_.push_back(std::move(bubble));
    }
  }
  FloatingBubble() = default;

  //   TOUCHABLE_OBJECT(FloatingBubble)

  void setPosition(const sf::Vector2f& position) {
    position_ = position;
    updateBubblesPosition();
  }
  const auto getPosition() const { return position_; }

  void setBubbleRadius(const float radius) {
    bubble_radius_ = radius;
    for (auto& bubble : bubbles_) {
      bubble->setRadius(radius);
    }
  }
  const auto getBubbleRadius() const { return bubble_radius_; }
  int getIndexByPosition(const sf::Vector2f& position) const;

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 private:
  sf::Vector2f position_;
  float distribute_radius_;
  int bubble_num_;
  float bubble_radius_;
  Direction direction_;
  std::vector<std::unique_ptr<BubbleItem>> bubbles_;

  sf::Vector2f getBubblePosition(const int index);
  void updateBubblesPosition();
};

}  // namespace yuki

#endif  // FLOATING_BUBBLE_HPP
