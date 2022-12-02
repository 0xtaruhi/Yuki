#include "Yuki.h"
#include "Soldier.h"

#include <memory>

using namespace yuki;

Yuki::Yuki(int window_width, int window_height)
    : window_width_(window_width),
      window_height_(window_height),
      window_(std::make_shared<sf::RenderWindow>(
          sf::VideoMode(window_width, window_height), "Yuki")),
      map_("/Users/taruhi/Project/Yuki/assets/beta.tmx", window_) {
        test_soldier_ = NormalSoldier(window_, sf::Vector2u(100, 100));
        test_soldier_.setDirection(Moving::Right);
        test_soldier_.setSpeed(1);
      }

Yuki::Yuki() : Yuki(kNormalWindowWidth, kNormalWindowHeight) {}

Yuki::~Yuki() {}

void Yuki::run() {
  while (window_->isOpen()) {
    while (window_->pollEvent(event_)) {
      if (event_.type == sf::Event::Closed) {
        window_->close();
      }
    }
    update();
  }
}

void Yuki::stop() {}

void Yuki::update() {
  window_->clear();
  map_.draw();
  test_soldier_.draw();
  test_soldier_.update();
  window_->display();
}
