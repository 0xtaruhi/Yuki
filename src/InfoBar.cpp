#include "InfoBar.hpp"

#include "Basic.hpp"

using namespace yuki;
using namespace sf;

InfoBar::InfoBar(const sf::Vector2f& position, const sf::Vector2f& size) {
  setPosition(position);
  background_.setSize(size);
  initTextures();
  initUi();
}

void InfoBar::setMoney(const int money) {
  text_money_.setString(std::to_string(money));
}

void InfoBar::setPosition(const sf::Vector2f& position) {
  background_.setPosition(position);
  icon_money_.setPosition(Vector2f(position.x + kMoneyIconOffset, position.y));
  text_money_.setPosition(Vector2f(position.x + kMoneyTextOffset, position.y));
}

void InfoBar::initTextures() {
  icon_money_texture_.loadFromFile("assets/res/blue_stone.png");
}

void InfoBar::initUi() {
  icon_money_.setTexture(icon_money_texture_);
  // background_.setTexture(&background_texture_);
  // * Temporary
  background_.setFillColor(Color::White);
  background_.setOutlineColor(Color::Black);
  background_.setOutlineThickness(3.f);
  // *

  text_money_.setFont(GameFont::DefaultFont);
  text_money_.setCharacterSize(24);
  text_money_.setColor(Color::Black);

  text_money_.setString("0");
}

void InfoBar::draw(RenderTarget& target, RenderStates states) const {
  target.draw(background_, states);
  target.draw(icon_money_, states);
  target.draw(text_money_, states);
}
