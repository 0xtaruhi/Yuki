#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "Sodier.hpp"
#include "Button.hpp"
#include "Elementum.hpp"

#include "InputProcessEngine.hpp"

using namespace yuki;

int main() {
  sf::RenderWindow window(sf::VideoMode(800, 600), "Yuki");
  NormalSodier sodier;
  sodier.setDirection(Direction::Down);
  sodier.setMoving(true);
  sodier.setSpeed(2.f);

  auto button = std::make_shared<Button>();
  button->setSize(sf::Vector2f(100, 50));
  button->setPosition(sf::Vector2f(100, 100));
  button->setString("Hello");
  button->setTextAlignment(Button::Alignment::Center);
  button->setTextFillColor(sf::Color::Red);
  button->autoAdjustTextSize();

  MouseEventProcessEngine mouse_event_engine;
  mouse_event_engine.registerTouchableObject(button);

  window.setFramerateLimit(60);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Left) {
          sodier.setDirection(Direction::Left);
        } else if (event.key.code == sf::Keyboard::Right) {
          sodier.setDirection(Direction::Right);
        } else if (event.key.code == sf::Keyboard::Up) {
          sodier.setDirection(Direction::Up);
        } else if (event.key.code == sf::Keyboard::Down) {
          sodier.setDirection(Direction::Down);
        } else if (event.key.code == sf::Keyboard::Space) {
          sodier.setMoving(!sodier.isMoving());
        }
      }
      mouse_event_engine.processEvent(event);
    }
    sodier.update();

    window.clear();
    window.draw(sodier);
    window.draw(*button);
    window.display();
  }

  return 0;
}
