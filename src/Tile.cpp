#include "Tile.hpp"

using namespace yuki;
using namespace sf;

std::string yuki::getTileCategoryName(const TileCategory category) {
  std::string res;
  switch (category) {
    case TileCategory::Building:
      res = "building";
      break;
    case TileCategory::Decor:
      res = "decor";
      break;
    case TileCategory::Greenery:
      res = "greenery";
      break;
    case TileCategory::Land:
      res = "land";
      break;
    case TileCategory::River:
      res = "river";
      break;
    case TileCategory::Road:
      res = "road";
      break;
    case TileCategory::Stones:
      res = "stones";
      break;
    case TileCategory::Tree:
      res = "tree";
      break;

    default:
      break;
  }
  return res;
}

std::string yuki::getTileSrcName(const TileInfo& tile_info) {
  std::string res(getTileCategoryName(tile_info.category));
  return res + "_" + std::to_string(tile_info.index);
}

sf::Vector2f yuki::getTileScaleFactor(const TileInfo& tile_info) {
  auto scale_factor = Vector2f({1.0, 1.0});

  switch (tile_info.category) {
    case TileCategory::Building:
      scale_factor = Vector2f({3.0, 3.0});
      break;
    case TileCategory::River:
      scale_factor = Vector2f({2.0, 2.0});
      break;
    case TileCategory::Tree:
      scale_factor = Vector2f({2.0, 2.0});
      break;
    default:
      break;
  }

  return scale_factor;
}
