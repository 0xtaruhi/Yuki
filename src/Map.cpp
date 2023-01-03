#include "Map.hpp"

using namespace sf;
using namespace yuki;

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
      scale_factor = Vector2f({2.0, 2.0});
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

Map::Map(const Vector2i& size, const Vector2f& tile_size)
    : size_(size), tile_size_(tile_size) {
  tiles_ = std::vector(size.x * size.y, Tile());
  bg_texture_.loadFromFile("assets/map/bg.png");
  bg_tiles_ = std::vector(size.x * size.y, Sprite(bg_texture_));

  auto bg_tile_factor = Vector2f({tile_size_.x / bg_texture_.getSize().x,
                                  tile_size_.y / bg_texture_.getSize().y});

  for (auto i = 0; i < size.x; ++i) {
    for (auto j = 0; j < size.y; ++j) {
      auto& tile = tiles_[j * size.x + i];
      // todo: set texture
      tile.setColor(Color::White);
      tile.setPosition(i * tile_size.x, j * tile_size.y);

      auto& bg_tile = bg_tiles_[j * size.x + i];
      bg_tile.setPosition(i * tile_size.x, j * tile_size.y);
      bg_tile.setScale(bg_tile_factor);
    }
  }
}

Map::~Map() {}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  for (auto& bg_tile : bg_tiles_) {
    target.draw(bg_tile, states);
  }
  for (auto& tile : tiles_) {
    target.draw(tile, states);
  }
}

Tile& Map::getTile(int x, int y) {
  if (!(x < size_.x && x >= 0 && y < size_.y && y >= 0)) {
    throw std::runtime_error("invalid tile position");
  }
  return tiles_[y * size_.x + x];
}

const Tile& Map::getTile(int x, int y) const {
  if (!(x < size_.x && x >= 0 && y < size_.y && y >= 0)) {
    throw std::runtime_error("invalid tile position");
  }
  return tiles_[y * size_.x + x];
}

void Map::setTile(int x, int y, const TileInfo& tile_info) {
  auto src_name = getTileSrcName(tile_info);
  auto& tile = getTile(x, y);
  tile.setTextureSrc("assets/map/" + src_name + ".png");
  auto texture_size = tile.getTexture()->getSize();
  auto scale_factor = getTileScaleFactor(tile_info);
  auto basic_scale = tile_size_.x / texture_size.x;
  tile.setScale(basic_scale * scale_factor);
}
