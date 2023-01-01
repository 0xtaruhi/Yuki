#include "YukiScene.hpp"

using namespace yuki;
using namespace sf;

YukiScene::~YukiScene() {}

void YukiScene::show() {
  while (window_.isOpen()) {
    sf::Event event;
    while (window_.pollEvent(event)) {
      processEvent(event);
    }
    if (notify_ == Notify::End) {
      break;
    }
    notify_ = Notify::None;
    window_.clear();
    draw();
    window_.display();
  }
}

void YukiScene::processEvent(sf::Event event) {
  if (event.type == sf::Event::Closed) {
    window_.close();
  }
  mouse_event_engine_.processEvent(event);
}
