#ifndef SKILL_HPP
#define SKILL_HPP

#include <SFML/Graphics.hpp>
#include <array>

#include "CircularSector.hpp"
#include "InteractiveObject.hpp"

namespace yuki {

constexpr auto kPi = 3.14159265358979323846f;
constexpr auto kSkillIconBorderDistance = 3.f;

class SkillIcon : public sf::Drawable, public Touchable {
 public:
  SkillIcon(const sf::Vector2f& position = {0, 0}, float radius = 10.f,
            float border_thick = 2.f);

  void setMaxValue(int max_value) { max_value_ = max_value; }
  constexpr auto getMaxValue() const { return max_value_; }
  void setCurrentValue(int current_value) { current_value_ = current_value; }
  constexpr auto getCurrentValue() const { return current_value_; }

  void setPosition(const sf::Vector2f& position);

  TOUCHABLE_OBJECT(SkillIcon)
 protected:
  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override;

 private:
  CircularSector circular_sector_;
  sf::CircleShape background_;
  sf::CircleShape icon_;

  int max_value_;
  int current_value_;

  void updateCircularSector();
};

template <std::size_t MaxSkillNum = 3>
class SkillBar : public sf::Drawable, public Touchable {
 public:
  SkillBar(const sf::Vector2f& position = {0, 0}, float radius = 10.f,
           float border_thick = 2.f, int skill_num = 0);

  constexpr auto getSkillNum() const { return skills_.size(); }

 protected:
  TOUCHABLE_OBJECT(SkillBar)
  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override;

 private:
  unsigned skill_num_;

  std::array<SkillIcon, MaxSkillNum> skills_;
  std::array<sf::RectangleShape, MaxSkillNum> boxes_;

  sf::Vector2f position_;

  float icon_radius_ = 10.f;
  float border_thick_ = 2.f;

  void updatePosition();
};

template <std::size_t N>
SkillBar<N>::SkillBar(const sf::Vector2f& position, float radius,
                      float border_thick, int skill_num)
    : skill_num_(skill_num),
      position_(position),
      icon_radius_(radius),
      border_thick_(border_thick) {
  for (auto& skill : skills_) {
    skill.setMaxValue(100);
    skill.setCurrentValue(33);
  }

  for (int i = 0; i != skill_num; ++i) {
    boxes_[i].setFillColor(sf::Color::Black);
    boxes_[i].setOutlineColor(sf::Color::White);
    boxes_[i].setOutlineThickness(2.f);
  }

  updatePosition();
}

template <std::size_t N>
void SkillBar<N>::draw(sf::RenderTarget& target,
                       sf::RenderStates states) const {
  for (const auto& box : boxes_) {
    target.draw(box, states);
  }

  for (const auto& skill : skills_) {
    target.draw(skill, states);
  }
}

template <std::size_t N>
void SkillBar<N>::updatePosition() {
  const float box_length = 2 * icon_radius_ + 2 * kSkillIconBorderDistance;

  for (std::size_t i = 0; i != skills_.size(); ++i) {
    boxes_[i].setPosition(position_.x + i * box_length, position_.y);

    skills_[i].setPosition(
        sf::Vector2f{static_cast<float>(position_.x + (i + 0.5) * box_length),
                     static_cast<float>(position_.y + 0.5 * box_length)});
  }
}

template <std::size_t N>
bool SkillBar<N>::inRange(const sf::Vector2f& point) const {
  auto box_length = 2 * icon_radius_ + 2 * kSkillIconBorderDistance;

  return point.x >= position_.x &&
         point.x <= position_.x + box_length * skill_num_ &&
         point.y >= position_.y &&
         point.y <= position_.y + box_length * skill_num_;
}

}  // namespace yuki

#endif  // SKILL_BAR_HPP
