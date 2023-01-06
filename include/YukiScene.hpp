#ifndef YUKI_SCENE_HPP
#define YUKI_SCENE_HPP

#include <SFML/Graphics.hpp>

#include "InputProcessEngine.hpp"

namespace yuki {

#define YUKI_SCENE                             \
 public:                                       \
  int show() override;                         \
                                               \
 protected:                                    \
  void processEvent(sf::Event event) override; \
  void draw() const override;                  \
  void updateInfo() override;                  \
                                               \
 public:

// Abstract class for all scenes
class YukiScene {
 public:
  enum class Notify { None, End };

  YukiScene(sf::RenderWindow& window) : window_(window) {}
  virtual ~YukiScene();

  virtual int show();

 protected:
  yuki::MouseEventProcessEngine mouse_event_engine_;
  Notify notify_;

  void registerTouchableObject(Touchable* object) {
    mouse_event_engine_.registerTouchableObject(object);
  }
  void unregisterTouchableObject(Touchable* object) {
    mouse_event_engine_.unregisterTouchableObject(object);
  }

  sf::RenderWindow& window_;
  virtual void processEvent(sf::Event event);
  virtual void draw() const = 0;
  virtual void updateInfo() = 0;
};

}  // namespace yuki

#endif  // YUKI_SCENE_HPP
