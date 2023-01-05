#include "StartupScene.hpp"

#include <SFML/Graphics.hpp>
#include <memory>

#include "Basic.hpp"

using namespace yuki;
using namespace sf;

StartupScene::StartupScene(sf::RenderWindow& window) : YukiScene(window) {
  initUi();
  registerTouchableObject(std::make_shared<Button>(btn_start_));
  registerTouchableObject(std::make_shared<Button>(btn_connect_));
}

int StartupScene::show() { return YukiScene::show(); }

void StartupScene::draw() const {
  window_.draw(background_);
  window_.draw(btn_start_);
  window_.draw(btn_connect_);
}

void StartupScene::processEvent(sf::Event event) {
  YukiScene::processEvent(event);
}

void StartupScene::updateInfo() {}

void StartupScene::initUi() {
  // Background
  background_texture_.loadFromFile("assets/image/start_bg.png");
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

  // Remote Connection button
  btn_connect_.setString("Remote Connection");
  btn_connect_.setSize(Vector2f(200,50));
  btn_connect_.setOrigin(Vector2f(100,25));
  btn_connect_.setPosition(kWindowWidth / 2,570);
  btn_connect_.setOutlineThickness(2);
  btn_connect_.setOutlineColor(Color::Black);

  btn_connect_.bindClick(
      [this](sf::Event) { btn_connect_.setTextFillColor(Color::Magenta); });
  btn_connect_.bindRelease([this](sf::Event) {
    btn_connect_.setTextFillColor(Color::Black);

    notify_ = Notify::End;
  });
  btn_connect_.bindHover([this](sf::Event) { btn_connect_.setFillColor(Color::Cyan); });
  btn_connect_.bindLeave([this](sf::Event) { btn_connect_.setFillColor(Color::White); });
}
