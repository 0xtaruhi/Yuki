#include "StartupScene.hpp"

#include <SFML/Graphics.hpp>
#include <memory>

#include "Basic.hpp"

using namespace yuki;
using namespace sf;

StartupScene::StartupScene(sf::RenderWindow& window) : YukiScene(window) {
  initUi();
  registerTouchableObject(std::make_shared<Button>(btn_start_));
}

int StartupScene::show() { return YukiScene::show(); }

void StartupScene::draw() const {
  window_.draw(background_);
  window_.draw(btn_start_);
}

void StartupScene::processEvent(sf::Event event) {
  YukiScene::processEvent(event);
}

void StartupScene::updateInfo() {}

void StartupScene::initUi() {
  // Background
  background_texture_.loadFromFile("assets/image/startup_bg.jpg");
  background_.setTexture(background_texture_);
  background_.setPosition(0, 0);

  // Start button
  btn_start_.setString("Start Game");
  btn_start_.setSize(Vector2f(200, 50));
  btn_start_.setOrigin(Vector2f(100, 25));
  btn_start_.setPosition(kWindowWidth / 2, 500);
  btn_start_.setOutlineThickness(2);
  btn_start_.setOutlineColor(Color::Black);

  btn_start_.bindClick(
      [this](sf::Event) { btn_start_.setTextFillColor(Color::Magenta); });
  btn_start_.bindRelease([this](sf::Event) {
    btn_start_.setTextFillColor(Color::Black);
    notify_ = Notify::End;
  });
  btn_start_.bindHover([this](sf::Event) { btn_start_.setFillColor(Color::Cyan); });
  btn_start_.bindLeave([this](sf::Event) { btn_start_.setFillColor(Color::White); });
}
