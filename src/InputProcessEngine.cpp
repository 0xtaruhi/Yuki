#include "InputProcessEngine.hpp"

#include <SFML/System.hpp>

using namespace sf;
using namespace yuki;

MouseEventProcessEngine::MouseEventProcessEngine() {}

MouseEventProcessEngine::~MouseEventProcessEngine() {}

void MouseEventProcessEngine::registerTouchableObject(Touchable* object) {
  touchable_objects_.push_back(object);
}

void MouseEventProcessEngine::unregisterTouchableObject(Touchable* object) {
  for (auto it = touchable_objects_.begin(); it != touchable_objects_.end();
       ++it) {
    if (*it == object) {
      touchable_objects_.erase(it);
      break;
    }
  }
}

void MouseEventProcessEngine::processEvent(sf::Event event) {
  for (auto& object : touchable_objects_) {
    object->updateStatus(event);
  }
}
