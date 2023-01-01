#include "Button.hpp"

#include "Basic.hpp"

using namespace yuki;
using namespace sf;

Button::Button() {
  text_.setFillColor(Color::Black);
  text_.setFont(GameFont::DefaultFont);
  text_.setCharacterSize(20);
}

Button::Button(const Vector2f& size) : Button() { shape_.setSize(size); }

Button::Button(const String& text, const Alignment alignment) : Button() {
  text_.setString(text);
  alignment_ = alignment;
}

Button::Button(const Vector2f& size, const String& text,
               const Alignment alignment)
    : Button(size) {
  text_.setString(text);
  alignment_ = alignment;
}

Button::~Button() {}

void Button::adjustTextPosition() {
  auto text_size = text_.getLocalBounds();
  auto text_pos = Vector2f(0, 0);

  switch (alignment_) {
    case Alignment::Left:
      text_pos.x = 0;
      break;
    case Alignment::Center:
      text_pos.x = (shape_.getSize().x - text_size.width) / 2;
      break;
    case Alignment::Right:
      text_pos.x = shape_.getSize().x - text_size.width;
      break;
  }

  text_pos.y = (shape_.getSize().y - text_size.height) / 2;
  text_pos += shape_.getPosition();
  text_.setPosition(text_pos);
}

void Button::autoAdjustTextSize() {
  auto char_length = text_.getString().getSize();
  auto char_size =
      std::min(shape_.getSize().x / char_length, shape_.getSize().y);

  text_.setCharacterSize(char_size);
}

void Button::draw(RenderTarget& target, RenderStates states) const {
  target.draw(shape_, states);
  target.draw(text_, states);
}

bool Button::inRange(const Vector2f& position) const {
  return shape_.getGlobalBounds().contains(position);
}

// void Button::onClick() {
//   shape_.setFillColor(Color::Blue);
// }

// void Button::onRelease() {
//   shape_.setFillColor(Color::White);
// }

// void Button::onHover() {
//   shape_.setFillColor(Color::Green);
// }

// void Button::onLeave() {
//   shape_.setFillColor(Color::White);
// }
