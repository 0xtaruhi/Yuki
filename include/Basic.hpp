#ifndef BASIC_HPP
#define BASIC_HPP

#include <SFML/System.hpp>
#include <iostream>

namespace yuki {

constexpr auto kWindowWidth = 960;
constexpr auto kWindowHeight = 640;

enum class Direction { Down = 0, Left, Right, Up, Other };

enum class Camp { Own, Enemy, Other };

namespace GameFont {
inline auto initFont(const sf::String& font_path) {
  sf::Font font;
  font.loadFromFile(font_path);
  if (font.getInfo().family == "") {
    std::cerr << "Font not found" << std::endl;
    throw std::runtime_error("Font not found");
  } else {
    std::cout << "Font loaded: " << font.getInfo().family << std::endl;
  }
  return font;
}

const static auto DefaultFont = initFont("assets/font/ARLRDBD.ttf");
}  // namespace GameFont

namespace YukiColor {
  const static sf::Color Transparent_50(255, 255, 255, 128);
  const static sf::Color Transparent(255, 255, 255, 0);
  const static sf::Color Transparent_25(255, 255, 255, 192);
  const static sf::Color Transparent_75(255, 255, 255, 64);
  const static sf::Color Normal(255, 255, 255, 255);
}

inline sf::Vector2f getVectorSpeed(const Direction direction,
                                   const float speed) {
  switch (direction) {
    case Direction::Down:
      return sf::Vector2f(0.f, speed);
    case Direction::Left:
      return sf::Vector2f(-speed, 0.f);
    case Direction::Right:
      return sf::Vector2f(speed, 0.f);
    case Direction::Up:
      return sf::Vector2f(0.f, -speed);
    default:
      return sf::Vector2f(0.f, 0.f);
  }
}

}  // namespace yuki

#endif
