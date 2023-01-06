#ifndef ELEMENTUM_PANEL_HPP
#define ELEMENTUM_PANEL_HPP

#include <SFML/Graphics.hpp>

#include "Elementum.hpp"
#include "InteractiveObject.hpp"

namespace yuki {
class ElementumPanel : public sf::CircleShape, public Touchable {
 public:
  ElementumPanel(ElementumType type = ElementumType::Pyro) {
    setElementumType(type);
  }

  TOUCHABLE_OBJECT(ElementumPanel)

  void activate();
  void deactivate();

  void setElementumType(ElementumType type);
  constexpr auto getElementumType() const { return elementum_type_; }

 private:
  ElementumType elementum_type_;
};

}  // namespace yuki

#endif
