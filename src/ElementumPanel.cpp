#include "ElementumPanel.hpp"

#include <cmath>

#include "Basic.hpp"


using namespace sf;
using namespace yuki;

void ElementumPanel::activate() {
  setOutlineColor(Color::Black);
  setOutlineThickness(4.0f);
}

void ElementumPanel::deactivate() {
  setOutlineThickness(1.0f);
  setOutlineColor(Color(128, 128, 128, 255));
}

void ElementumPanel::setElementumType(ElementumType type) {
  elementum_type_ = type;
  switch (type) {
    case ElementumType::Pyro:
      setFillColor(Color::Red);
      break;
    case ElementumType::Hydro:
      setFillColor(Color::Blue);
      break;
    case ElementumType::Cyro:
      setFillColor(Color::White);
      break;
    default:
      setFillColor(YukiColor::Gray);
      break;
  }
}

bool ElementumPanel::inRange(const Vector2f& point) const {
  return getGlobalBounds().contains(point);
}
