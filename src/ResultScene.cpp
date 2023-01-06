#include "ResultScene.hpp"

#include "Basic.hpp"

using namespace yuki;
using namespace sf;

ResultScene::ResultScene(sf::RenderWindow& window, bool own_win)
    : YukiScene(window), own_win_(own_win) {
  initUi();
}

ResultScene::~ResultScene() {}

int ResultScene::show() { return YukiScene::show(); }

void ResultScene::draw() const {
  window_.draw(background_);
  window_.draw(result_text_);
}

void ResultScene::processEvent(sf::Event event) {
  YukiScene::processEvent(event);
}

void ResultScene::updateInfo() {}

void ResultScene::initUi() {
  result_text_.setFont(GameFont::DefaultFont);
  result_text_.setCharacterSize(40);

  result_text_.setOrigin(result_text_.getLocalBounds().width / 2,
                         result_text_.getLocalBounds().height / 2);

  result_text_.setPosition({static_cast<float>(window_.getSize().x) / 2 - 50.f,
                            static_cast<float>(window_.getSize().y) / 2});

  if (own_win_) {
    result_text_.setString("You Win!");
    result_text_.setFillColor(Color::Green);
  } else {
    result_text_.setString("You Lose!");
    result_text_.setFillColor(Color::Red);
  }
}
