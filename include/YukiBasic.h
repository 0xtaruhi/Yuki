#ifndef YUKI_BASIC_H
#define YUKI_BASIC_H

#include <SFML/Graphics.hpp>

namespace yuki {
using WindowHandler = std::shared_ptr<sf::RenderWindow>;

constexpr auto kNormalWindowWidth = 1024;
constexpr auto kNormalWindowHeight = 768;

constexpr auto cellLength = 16;

enum class Moving {
  Left = 0,
  Right,
  Up,
  Down,
  Stop,
  Other
};

};

#endif