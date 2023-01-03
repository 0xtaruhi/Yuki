#include "YukiScene.hpp"

using namespace yuki;
using namespace sf;

YukiScene::~YukiScene() {}

int YukiScene::show() {
  int ret_code = 0;
  while (window_.isOpen()) {
    sf::Event event;
    while (window_.pollEvent(event)) {
      processEvent(event);
    }
    if (notify_ == Notify::End) {
      ret_code = 1;
      break;
    }
    notify_ = Notify::None;
    updateInfo();
    window_.clear();
    draw();
    window_.display();
  }
  return ret_code;
}

void YukiScene::processEvent(sf::Event event) {
  if (event.type == sf::Event::Closed) {
    window_.close();
  }
  mouse_event_engine_.processEvent(event);
}
