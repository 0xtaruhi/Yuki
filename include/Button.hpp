#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>

#include "InteractiveObject.hpp"
#include "YukiScene.hpp"

namespace yuki {

class Button : public sf::Drawable, public Touchable {
 public:
  enum class Alignment { Left, Center, Right };

  Button();
  Button(const sf::Vector2f& size);
  Button(const sf::String& text, const Alignment alignment = Alignment::Center);
  Button(const sf::Vector2f& size, const sf::String& text,
         const Alignment Alignment = Alignment::Center);
  template <typename... Args>
  Button(YukiScene& scene, Args&&... args)
      : Button(std::forward<Args>(args)...) {
    scene.registerTouchableObject(std::make_shared<Button>(*this));
  }

  virtual ~Button();

  // Interact
  TOUCHABLE_OBJECT
  virtual void onClick() override;
  virtual void onRelease() override;
  virtual void onHover() override;
  virtual void onLeave() override;

  // Background
  void setSize(const sf::Vector2f& size) {
    shape_.setSize(size);
    adjustTextPosition();
  }
  const auto getSize() const { return shape_.getSize(); }

  void setPosition(const sf::Vector2f& position) {
    shape_.setPosition(position);
    adjustTextPosition();
  }
  void setPosition(float x, float y) { setPosition(sf::Vector2f(x, y)); }

  const auto getPosition() const { return shape_.getPosition(); }

  void setFillColor(const sf::Color& color) { shape_.setFillColor(color); }

  void setOutlineColor(const sf::Color& color) {
    shape_.setOutlineColor(color);
  }

  void setOutlineThickness(float thickness) {
    shape_.setOutlineThickness(thickness);
  }

  void setOrigin(const sf::Vector2f& origin) { shape_.setOrigin(origin); }
  const auto getOrigin() const { return shape_.getOrigin(); }

  void setTexture(const sf::Texture* texture) { shape_.setTexture(texture); }
  const auto getTexture() const { return shape_.getTexture(); }

  void setTextureRect(const sf::IntRect& rect) { shape_.setTextureRect(rect); }

  // Text
  void setString(const sf::String& text) { text_.setString(text); }
  const auto getString() const { text_.getString(); }
  void setTextSize(unsigned int size) { text_.setCharacterSize(size); }
  void setTextFont(const sf::Font& font) { text_.setFont(font); }
  void setTextFillColor(const sf::Color& color) { text_.setFillColor(color); }
  void setTextOutlineColor(const sf::Color& color) {
    text_.setOutlineColor(color);
  }
  void setTextOutlineThickness(float thickness) {
    text_.setOutlineThickness(thickness);
  }
  void setTextAlignment(Alignment alignment) {
    alignment_ = alignment;
    adjustTextPosition();
  }

  void autoAdjustTextSize();

  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override;

 private:
  sf::RectangleShape shape_;
  sf::Text text_;

  Alignment alignment_ = Alignment::Center;
  void adjustTextPosition();
};

}  // namespace yuki

#endif
