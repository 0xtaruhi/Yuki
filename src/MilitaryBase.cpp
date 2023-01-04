#include "MilitaryBase.hpp"

#include "Tile.hpp"

using namespace yuki;
using namespace sf;

MilitaryBase::MilitaryBase() { initTexture(); }

void MilitaryBase::initTexture() { updateTexture(); }

void MilitaryBase::updateTexture() {
  auto getTextureLoc = [](int level) {
    return "assets/map/" + getTileSrcName(TileInfo(TileCategory::Building, level)) + ".png";
  };
  if (level >= 1 && level <= 3) {
    texture_.loadFromFile(getTextureLoc(level));
  } else {
    texture_.loadFromFile(getTextureLoc(1));
  }

  setTexture(texture_);
  setScale({0.4, 0.4});
}

bool MilitaryBase::inRange(const sf::Vector2f& position) const {
  return getGlobalBounds().contains(position);
}
