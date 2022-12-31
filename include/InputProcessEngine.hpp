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

  void registerTouchableObject(std::shared_ptr<Touchable> object);
  void unregisterTouchableObject(std::shared_ptr<Touchable> object);

  void processEvent(sf::Event event);

 private:
  std::vector<std::shared_ptr<Touchable>> touchable_objects_;
};

}  // namespace yuki

#endif
