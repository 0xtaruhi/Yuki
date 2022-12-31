#ifndef INTERACTIVE_OBJECT_HPP
#define INTERACTIVE_OBJECT_HPP

#include <SFML/Graphics.hpp>

#define TOUCHABLE_OBJECT \
  virtual bool inRange(const sf::Vector2f& position) const override;

namespace yuki {

class Touchable {
 public:
  Touchable() = default;
  virtual ~Touchable() {}

  virtual void onRelease() {}
  virtual void onClick() {}
  virtual void onHover() {}
  virtual void onLeave() {}

  virtual bool inRange(const sf::Vector2f& position) const = 0;

  void updateStatus(sf::Event event) {
    if (event.type == sf::Event::MouseButtonPressed) {
      if (inRange(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
        is_clicked_ = true;
        onClick();
      }
    } else if (event.type == sf::Event::MouseButtonReleased) {
      if (is_clicked_) {
        is_clicked_ = false;
        onRelease();
        if (inRange(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
          onHover();
          is_hovered_ = true;
        }
      }
    } else if (!is_clicked_ && event.type == sf::Event::MouseMoved) {
      if (!is_hovered_ &&
          inRange(sf::Vector2f(event.mouseMove.x, event.mouseMove.y))) {
        onHover();
        is_hovered_ = true;
      } else if (is_hovered_ &&
                 !inRange(sf::Vector2f(event.mouseMove.x, event.mouseMove.y))) {
        onLeave();
        is_hovered_ = false;
      }
    }
  }

 protected:
  bool is_clicked_ = false;
  bool is_hovered_ = false;
};

}  // namespace yuki

#endif  // INTERACTIVE_OBJECT_HPP
