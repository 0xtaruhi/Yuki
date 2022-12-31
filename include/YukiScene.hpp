#ifndef YUKI_SCENE_HPP
#define YUKI_SCENE_HPP

#include <SFML/Graphics.hpp>

#include "InputProcessEngine.hpp"

namespace yuki {

// Abstract class for all scenes
class YukiScene {
  friend class Button;

 public:
  YukiScene(sf::RenderWindow& window) : window_(window) {}
  virtual ~YukiScene();

  virtual void show();

 protected:
  yuki::MouseEventProcessEngine mouse_event_engine_;

  void registerTouchableObject(std::shared_ptr<Touchable> object) {
    mouse_event_engine_.registerTouchableObject(object);
  }

  sf::RenderWindow& window_;
  void processEvent(sf::Event event);

  virtual void draw() const = 0;

};

}  // namespace yuki

#endif  // YUKI_SCENE_HPP
