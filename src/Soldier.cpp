#include "Soldier.hpp"

#include <iostream>

using namespace yuki;
using namespace sf;

void Soldier::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(sprite_, states);
  target.draw(health_bar_, states);
  if (floating_bubble_visible_) {
    target.draw(floating_bubble_, states);
  }
}

bool Soldier::inRange(const Vector2f& target) const {
  return sprite_.getGlobalBounds().contains(target);
}

void Soldier::update() {
  if (is_moving_) {
    sprite_.move(getVectorSpeed(direction_, speed_));
  }
  // health_bar_.setPosition(
  //     {sprite_.getPosition().x, sprite_.getPosition().y - 15});
  health_bar_.setPosition({sprite_.getPosition().x + health_bar_offset_.x,
                           sprite_.getPosition().y + health_bar_offset_.y});
}

NormalSoldier::NormalSoldier(Camp camp) : Soldier(camp) {
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

NormalSoldier::NormalSoldier(const sf::Vector2f& position, Camp camp)
    : NormalSoldier(camp) {
  setPosition(position);
}

void NormalSoldier::draw(sf::RenderTarget& target,
                         sf::RenderStates states) const {
  Soldier::draw(target, states);
}

void NormalSoldier::update() {
  Soldier::update();

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
  if (clock_.getElapsedTime().asSeconds() > 1.f) {
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

  auto color = sprite_.getColor();
  switch (elementum_.type) {
    case ElementumType::Pyro:
      color.g = 128;
      color.b = 128;
      color.r = 255;
      break;
    case ElementumType::Hydro:
      color.r = 128;
      color.b = 255;
      color.g = 128;
      break;
    case ElementumType::Cyro:
      color.r = 128;
      color.g = 255;
      color.b = 128;
      break;
    default:
      break;
  }
  sprite_.setColor(color);

  if (is_freeze_) {
    if (freeze_clock_.getElapsedTime().asSeconds() > 3.f) {
      is_freeze_ = false;
      setMoving(true);
    }
  }
}

std::unique_ptr<NormalSoldier> yuki::getNormalSoldier(Camp camp) {
  auto soldier = std::make_unique<NormalSoldier>(camp);

  soldier->setSpeed(1.f);
  soldier->setDirection(Direction::Up);

  soldier->setMoving(true);
  soldier->setMaxHealth(100.f);
  soldier->setHealth(100.f);

  return soldier;
}

std::unique_ptr<Soldier> yuki::getSoldier(const std::string& name, Camp camp) {
  if (name == "NormalSoldier") {
    return getNormalSoldier(camp);
  }
  return nullptr;
}

void Soldier::getAttacked(const BasicAttack& attack) {
  auto elementum_reaction =
      getElementumReaction(attack.getElementum(), adhesion_elementum_);
  const auto& reaction_type = elementum_reaction.type;
  const auto& result_elementum = elementum_reaction.elementum;

  const auto& base_damage = attack.getDamage();
  auto result_damage = base_damage;

  adhesion_elementum_ = result_elementum;

  switch (reaction_type) {
    case ElementumReactionType::Evaporation:
      result_damage *= 1.5f;
      break;
    case ElementumReactionType::Melt:
      result_damage *= 2.f;
      break;
    case ElementumReactionType::Freeze:
      is_freeze_ = true;
      setMoving(false);
      freeze_clock_.restart();
      break;
    default:
      break;
  }

  decreaseHealth(result_damage);
}

std::unique_ptr<GranuleAttack> NormalSoldier::getDefaultGranuleAttack() {
  return std::make_unique<GranuleAttack>(
      Vector2f{getPosition().x + getSize().x / 2,
               getPosition().y + getSize().y / 2},
      5.0f, getDirection(), AttackInfo(getElementumType(), 2.0, 2.0));
}
