#ifndef INPUT_PROCESS_ENGINE_HPP
#define INPUT_PROCESS_ENGINE_HPP

#include <memory>
#include <vector>

#include "InteractiveObject.hpp"


namespace yuki {

class MouseEventProcessEngine {
 public:
  MouseEventProcessEngine();
  virtual ~MouseEventProcessEngine();

  void registerTouchableObject(Touchable* object);
  void unregisterTouchableObject(Touchable* object);

  void processEvent(sf::Event event);

 private:
  std::vector<Touchable*> touchable_objects_;
};

}  // namespace yuki

#endif
