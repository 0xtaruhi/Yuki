#include "Sodier.hpp"

#include <iostream>

using namespace yuki;
using namespace sf;

bool Sodier::inRange(const Vector2f& target) const {
  return sprite_.getGlobalBounds().contains(target);
}

void Sodier::update() {
  if (is_moving_) {
    sprite_.move(getVectorSpeed(direction_, speed_));
  }
  // health_bar_.setPosition(
  //     {sprite_.getPosition().x, sprite_.getPosition().y - 15});
  health_bar_.setPosition({sprite_.getPosition().x + health_bar_offset_.x,
                           sprite_.getPosition().y + health_bar_offset_.y});
}

NormalSodier::NormalSodier(Camp camp) : Sodier(camp) {
  current_frame_ = 0;
  max_frame_ = 3;

  std::string res_loc("assets/character/");
  if (camp == Camp::Own) {
    res_loc += "dragon_blue.png";
  } else {
    res_loc += "dragon_red.png";
  }

  // Load the texture
  if (!all_textures_.loadFromFile(res_loc)) {
    std::cout << "Failed to load texture" << std::endl;
  }
  sprite_.setTexture(all_textures_);
  sprite_.setPosition(100, 100);
  sprite_.setTextureRect(sf::IntRect(0, 0, 42, 42));
}

NormalSodier::NormalSodier(const sf::Vector2f& position, Camp camp)
    : NormalSodier(camp) {
  setPosition(position);
}

void NormalSodier::draw(sf::RenderTarget& target,
                        sf::RenderStates states) const {
  target.draw(sprite_, states);
  target.draw(health_bar_, states);
}

void NormalSodier::update() {
  Sodier::update();

  int direct_int;
  switch (direction_) {
    case Direction::Down:
      direct_int = 0;
      break;
    case Direction::Left:
      direct_int = 1;
      break;
    case Direction::Right:
      direct_int = 2;
      break;
    case Direction::Up:
      direct_int = 3;
      break;
    default:
      direct_int = 0;
      break;
  }
  if (clock_.getElapsedTime().asMilliseconds() > 100) {
    clock_.restart();
    if (is_moving_ == false) {
      sprite_.setTextureRect(sf::IntRect(42, direct_int * 42, 42, 42));
    } else {
      sprite_.setTextureRect(
          sf::IntRect(current_frame_ * 42, direct_int * 42, 42, 42));
    }
  }

  current_frame_++;
  if (current_frame_ == max_frame_) {
    current_frame_ = 0;
  }

  switch (elementum_.type) {
    case ElementumType::Pyro:
      sprite_.setColor(sf::Color::Red);
      break;
    case ElementumType::Hydro:
      sprite_.setColor(sf::Color::Blue);
      break;
    case ElementumType::Cyro:
      sprite_.setColor(sf::Color::White);
      break;
    default:
      break;
  }
}
