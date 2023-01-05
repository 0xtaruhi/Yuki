#include "InteractiveObject.hpp"

using namespace sf;
using namespace yuki;

void Touchable::bindAction(const Action action, Callback callback) {
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

void Touchable::updateStatus(sf::Event event) {
  if (event.type == sf::Event::MouseButtonPressed) {
    if (inRange(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
      is_clicked_ = true;
      onClick(event);
    }
  } else if (event.type == sf::Event::MouseButtonReleased) {
    if (is_clicked_) {
      is_clicked_ = false;
      onRelease(event);
      if (inRange(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
        onHover(event);
        is_hovered_ = true;
      }
    }
  } else if (!is_clicked_ && event.type == sf::Event::MouseMoved) {
    if (!is_hovered_ &&
        inRange(sf::Vector2f(event.mouseMove.x, event.mouseMove.y))) {
      onHover(event);
      is_hovered_ = true;
    } else if (is_hovered_ &&
               !inRange(sf::Vector2f(event.mouseMove.x, event.mouseMove.y))) {
      onLeave(event);
      is_hovered_ = false;
    }
  }
}

void Focusable::updateStatus(sf::Event event) {
  if (event.type == sf::Event::MouseButtonPressed) {
    if (inRange(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
      is_clicked_ = true;
      onClick(event);
      if (!is_focused_) {
        onFocus(event);
        is_focused_ = true;
      }
    } else if (is_focused_) {
      onUnfocus(event);
      is_focused_ = false;
    }
  } else if (event.type == sf::Event::MouseButtonReleased) {
    if (is_clicked_) {
      is_clicked_ = false;
      onRelease(event);
      if (inRange(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
        onHover(event);
        is_hovered_ = true;
      }
    }
  } else if (!is_clicked_ && event.type == sf::Event::MouseMoved) {
    if (!is_hovered_ &&
        inRange(sf::Vector2f(event.mouseMove.x, event.mouseMove.y))) {
      onHover(event);
      is_hovered_ = true;
    } else if (is_hovered_ &&
               !inRange(sf::Vector2f(event.mouseMove.x, event.mouseMove.y))) {
      onLeave(event);
      is_hovered_ = false;
    }
  }
}
