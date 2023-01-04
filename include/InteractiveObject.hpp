#ifndef INTERACTIVE_OBJECT_HPP
#define INTERACTIVE_OBJECT_HPP

#include <SFML/Graphics.hpp>
#include <functional>

#define TOUCHABLE_OBJECT(OBJECT_NAME)                                \
 private:                                                            \
  virtual bool inRange(const sf::Vector2f& position) const override; \
                                                                     \
 public:

namespace yuki {

class Touchable {
 public:
  using Callback = std::function<void()>;

  Touchable() = default;
  virtual ~Touchable() {}

  virtual void onRelease() { on_releaseCallback_(); }
  virtual void onClick() { on_clickCallback_(); }
  virtual void onHover() { on_hoverCallback_(); }
  virtual void onLeave() { on_leaveCallback_(); }

  enum class Action { Click, Release, Hover, Leave };
  void bindAction(const Action action, Callback callback) {
    switch (action) {
      case Action::Click:
        on_clickCallback_ = callback;
        break;
      case Action::Release:
        on_releaseCallback_ = callback;
        break;
      case Action::Hover:
        on_hoverCallback_ = callback;
        break;
      case Action::Leave:
        on_leaveCallback_ = callback;
        break;
    }
  }

  void bindClick(Callback callback) { on_clickCallback_ = callback; }
  void bindRelease(Callback callback) { on_releaseCallback_ = callback; }
  void bindHover(Callback callback) { on_hoverCallback_ = callback; }
  void bindLeave(Callback callback) { on_leaveCallback_ = callback; }

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

  Callback on_clickCallback_ = []() {};
  Callback on_releaseCallback_ = []() {};
  Callback on_hoverCallback_ = []() {};
  Callback on_leaveCallback_ = []() {};
};

}  // namespace yuki

#endif  // INTERACTIVE_OBJECT_HPP
