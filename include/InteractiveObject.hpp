#ifndef INTERACTIVE_OBJECT_HPP
#define INTERACTIVE_OBJECT_HPP

#include <SFML/Graphics.hpp>
#include <functional>

#define TOUCHABLE_OBJECT(OBJECT_NAME) \
 public:                              \
  virtual bool inRange(const sf::Vector2f& position) const override;

#define FOCUSABLE_OBJECT(OBJECT_NAME) TOUCHABLE_OBJECT(OBJECT_NAME)

namespace yuki {

class Touchable {
 public:
  using Callback = std::function<void(sf::Event e)>;
  // using Callback = void (*)(sf::Event e);

  Touchable() = default;
  virtual ~Touchable() {}

  virtual void onRelease(sf::Event e) { on_releaseCallback_(e); }
  virtual void onClick(sf::Event e) { on_clickCallback_(e); }
  virtual void onHover(sf::Event e) { on_hoverCallback_(e); }
  virtual void onLeave(sf::Event e) { on_leaveCallback_(e); }

  enum class Action { Click, Release, Hover, Leave };

  void bindAction(const Action action, Callback callback);

  void bindClick(Callback callback) { on_clickCallback_ = callback; }
  void bindRelease(Callback callback) { on_releaseCallback_ = callback; }
  void bindHover(Callback callback) { on_hoverCallback_ = callback; }
  void bindLeave(Callback callback) { on_leaveCallback_ = callback; }

  virtual bool inRange(const sf::Vector2f& position) const = 0;
  virtual void updateStatus(sf::Event event);

 protected:
  bool is_clicked_ = false;
  bool is_hovered_ = false;

  Callback on_clickCallback_ = [](sf::Event) {};
  Callback on_releaseCallback_ = [](sf::Event) {};
  Callback on_hoverCallback_ = [](sf::Event) {};
  Callback on_leaveCallback_ = [](sf::Event) {};
};

class Focusable : public Touchable {
 public:
  using Callback = std::function<void(sf::Event e)>;

  Focusable() = default;
  virtual ~Focusable() {}

  virtual void onFocus(sf::Event e) { on_focusCallback_(e); }
  virtual void onUnfocus(sf::Event e) { on_unfocusCallback_(e); }

  constexpr auto isFocused() const { return is_focused_; }

  void bindFocus(Callback callback) { on_focusCallback_ = callback; }
  void bindUnfocus(Callback callback) { on_unfocusCallback_ = callback; }

  void updateStatus(sf::Event event);

 private:
  bool is_focused_ = false;

  Callback on_focusCallback_ = [](sf::Event) {};
  Callback on_unfocusCallback_ = [](sf::Event) {};
};

template <class Decorated>
class TouchableObject : public Decorated, public Touchable {
 public:
  TouchableObject() = default;
  virtual ~TouchableObject() {}

  virtual bool inRange(const sf::Vector2f& position) const override {
    return Decorated::getGlobalBounds().contains(position);
  }

  virtual void updateStatus(sf::Event event) override {
    Touchable::updateStatus(event);
  }
};

}  // namespace yuki

#endif  // INTERACTIVE_OBJECT_HPP
