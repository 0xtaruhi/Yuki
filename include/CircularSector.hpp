#ifndef CIRCULAR_SECTOR_HPP
#define CIRCULAR_SECTOR_HPP

#include <SFML/Graphics.hpp>

namespace yuki {

class CircularSector : public sf::Shape {
 public:
  CircularSector(const sf::Vector2f& center, float radius, int start_angle,
                 int end_angle)
      : center_(center),
        radius_(radius),
        start_angle_(start_angle),
        end_angle_(end_angle) {
    update();
  }

  void setCenter(const sf::Vector2f& center) {
    center_ = center;
    update();
  }

  void setRadius(float radius) {
    radius_ = radius;
    update();
  }

  void setStartAngle(float start_angle) {
    start_angle_ = start_angle;
    update();
  }

  void setEndAngle(float end_angle) {
    end_angle_ = end_angle;
    update();
  }

  const sf::Vector2f& getCenter() const { return center_; }

  sf::Vector2f getPoint(std::size_t index) const override;
  std::size_t getPointCount() const override {
    return end_angle_ - start_angle_ + 2;
  }

 private:
  sf::Vector2f center_;
  float radius_;
  int start_angle_;
  int end_angle_;
};

}  // namespace yuki

#endif  // CIRCULAR_SECTOR_HPP
