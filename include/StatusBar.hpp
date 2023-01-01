#ifndef STATUS_BAR_HPP
#define STATUS_BAR_HPP

#include <SFML/Graphics.hpp>

namespace yuki {

class StatusBar : public sf::Drawable {
 public:
  StatusBar() = default;
  StatusBar(sf::Vector2f size, sf::Vector2f position,
            sf::Color background_color = sf::Color::Black,
            sf::Color filled_color = sf::Color::Blue, float max_value = 100.f,
            float current_value = 100.f)
      : size_(size),
        position_(position),
        max_value_(max_value),
        current_value_(current_value) {
    background_.setPosition(position_);
    filled_.setPosition(position_);
    background_.setSize(size_);
    background_.setFillColor(background_color);
    filled_.setFillColor(filled_color);

    visible_ = true;

    updateFilledPercentage();
  }

  void show() { visible_ = true; }
  void hide() { visible_ = false; }

  auto isVisible() const { return visible_; }

  void setBackgroudColor(const sf::Color color) { background_.setFillColor(color); }
  void setFilledColor(const sf::Color color) { filled_.setFillColor(color); }
  void setMaxValue(const float max_value) { max_value_ = max_value; }
  void setCurrentValue(const float current_value) {
    current_value_ = current_value;
    updateFilledPercentage();
  }
  void setPosition(const sf::Vector2f& position);

  void setSize(const sf::Vector2f& size) {
    size_ = size;
    background_.setSize(size);
    updateFilledPercentage();
  }

  void setOutlineThickness(const float thickness) {
    background_.setOutlineThickness(thickness);
  }

  void setOutlineColor(const sf::Color& color) {
    background_.setOutlineColor(color);
  }

  void setOutlineStyle(const sf::Color& color, const float thickness) {
    background_.setOutlineColor(color);
    background_.setOutlineThickness(thickness);
  }

  const auto getBackgroundColor() const { return background_.getFillColor(); }
  const auto getFilledColor() const { return filled_.getFillColor(); }
  const auto getMaxValue() const { return max_value_; }
  const auto getCurrentValue() const { return current_value_; }
  const auto getPosition() const { return position_; }

  void increase(const float value) {
    current_value_ += value;
    updateFilledPercentage();
  }
  void decrease(const float value) {
    current_value_ -= value;
    updateFilledPercentage();
  }

  virtual ~StatusBar() {}

  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override;

  void updateFilledPercentage() {
    setFilledPercentage(current_value_ / max_value_);
  }

 private:
  sf::RectangleShape background_;
  sf::RectangleShape filled_;

  sf::Vector2f size_;
  sf::Vector2f position_;

  float max_value_;
  float current_value_;

  bool visible_;

  void setFilledPercentage(float percentage);
};

inline auto getHealthBar(const sf::Vector2f& size = {30.f, 5.f}) {
  auto health_bar = StatusBar(size, {0.f, 0.f});
  health_bar.setBackgroudColor(sf::Color::Black);
  health_bar.setFilledColor(sf::Color::Red);
  
  // set Outline
  health_bar.setOutlineStyle(sf::Color::White, 1.f);

  return health_bar;
}

}  // namespace yuki

#endif  // STATUS_BAR_HPP