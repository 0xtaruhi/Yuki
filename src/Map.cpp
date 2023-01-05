#include "Map.hpp"

using namespace sf;
using namespace yuki;

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

Tile& Map::getTile(const sf::Vector2i& coordinate) {
  return getTile(coordinate.x, coordinate.y);
}

const Tile& Map::getTile(const sf::Vector2i& coordinate) const {
  return getTile(coordinate.x, coordinate.y);
}

void Map::setTile(int x, int y, const TileInfo& tile_info) {
  auto src_name = getTileSrcName(tile_info);
  auto& tile = getTile(x, y);
  tile.setTextureSrc("assets/map/" + src_name + ".png");
  auto texture_size = tile.getTexture()->getSize();
  auto scale_factor = getTileScaleFactor(tile_info);
  auto basic_scale = tile_size_.x / texture_size.x;
  tile.setScale(basic_scale * scale_factor);
  tile.setTileInfo(tile_info);
}
