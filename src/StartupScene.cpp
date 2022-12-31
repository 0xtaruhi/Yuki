#include "StartupScene.hpp"

#include <SFML/Graphics.hpp>
#include <memory>

using namespace yuki;
using namespace sf;

StartupScene::StartupScene(sf::RenderWindow& window) : YukiScene(window) {
  auto btn_start = std::make_unique<Button>(*this, String("Start"));
  btn_start->setSize(Vector2f(100, 50));
  btn_start->setTextFillColor(Color::Red);
  btn_start->setTextAlignment(Button::Alignment::Center);

  menu_.addItem(std::move(btn_start));
}

void StartupScene::show() {
  YukiScene::show();
}

void StartupScene::draw() const {
  window_.draw(menu_);
}
