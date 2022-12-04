#include "Map.h"

#include <iostream>
#include <memory>

#include "tmxlite/Layer.hpp"
#include "tmxlite/Map.hpp"
#include "tmxlite/TileLayer.hpp"

using namespace yuki;

Map::Map(const std::string& map_path, WindowHandler window)
    : parent_window_(window) {
  map_.load(map_path);
  auto tileset = map_.getTilesets().back();

  tile_size_ = tileset.getTileSize();
  const auto tileset_size = tileset.getImageSize();

  const auto width = tileset_size.x / tile_size_.x;
  const auto height = tileset_size.y / tile_size_.y;

  sf::Image tileset_image;
  tileset_image.loadFromFile(tileset.getImagePath());

  for (int i = 0; i != height; ++i) {
    for (int j = 0; j != width; ++j) {
      textures_.push_back(std::make_unique<sf::Texture>());
      auto& cur_texture = textures_.back();
      cur_texture->loadFromImage(tileset_image,
                                 sf::IntRect(j * tile_size_.x, i * tile_size_.y,
                                             tile_size_.x, tile_size_.y));
    }
  }
}

void Map::draw() const {
  const auto& layers = map_.getLayers();
  const auto& tile_size = map_.getTileSize();

  for (const auto& layer : layers) {
    const auto& layer_size = layer->getSize();

    if (layer->getType() == tmx::Layer::Type::Tile) {
      const auto& tiles = layer->getLayerAs<tmx::TileLayer>().getTiles();

      for (size_t i = 0; i != layer_size.y; ++i) {
        for (size_t j = 0; j != layer_size.x; ++j) {
          const auto& tile = tiles[i * layer_size.x + j];
          auto shape = getShapeByTile(tile);
          shape.setPosition(j * tile_size.x * scale_.x, i * tile_size.y * scale_.y);
          parent_window_->draw(shape);
        }
      }
    }
  }
}

void Map::setScale(const sf::Vector2f& scale) { scale_ = scale; }

sf::RectangleShape Map::getShapeByTile(const tmx::TileLayer::Tile& tile) const {
  sf::RectangleShape temp_rec;
  temp_rec.setSize(sf::Vector2f(tile_size_.x, tile_size_.y));
  temp_rec.setTexture(textures_[tile.ID - 1].get());
  doFlip(tile, temp_rec);
  temp_rec.setScale(scale_);
  return temp_rec;
}

void Map::doFlip(const tmx::TileLayer::Tile& tile,
                 sf::RectangleShape& shape) const {
  const auto& flip_type = tile.flipFlags;

  auto doXFlip = [&shape]() {
    shape.setScale(-1, 1);
    // shape.setOrigin(shape.getSize().x, 0);
  };

  auto doYFlip = [&shape]() {
    shape.setScale(1, -1);
    // shape.setOrigin(0, shape.getSize().y);
  };

  auto doDFlip = [&shape]() {
    shape.setScale(-1, -1);
    // shape.setOrigin(shape.getSize().x, shape.getSize().y);
  };

  if (!(flip_type & tmx::TileLayer::FlipFlag::Horizontal) &&
      !(flip_type & tmx::TileLayer::FlipFlag::Vertical) &&
      !(flip_type & tmx::TileLayer::FlipFlag::Diagonal)) {
    return;
  } else if (!(flip_type & tmx::TileLayer::FlipFlag::Horizontal) &&
             (flip_type & tmx::TileLayer::FlipFlag::Vertical) &&
             !(flip_type & tmx::TileLayer::FlipFlag::Diagonal)) {
    // 0100
    doYFlip();
  } else if ((flip_type & tmx::TileLayer::FlipFlag::Horizontal) &&
             !(flip_type & tmx::TileLayer::FlipFlag::Vertical) &&
             !(flip_type & tmx::TileLayer::FlipFlag::Diagonal)) {
    // 1000
    doXFlip();
  } else if ((flip_type & tmx::TileLayer::FlipFlag::Horizontal) &&
             (flip_type & tmx::TileLayer::FlipFlag::Vertical) &&
             !(flip_type & tmx::TileLayer::FlipFlag::Diagonal)) {
    // 1100
    doYFlip();
    doXFlip();
  } else if (!(flip_type & tmx::TileLayer::FlipFlag::Horizontal) &&
             !(flip_type & tmx::TileLayer::FlipFlag::Vertical) &&
             (flip_type & tmx::TileLayer::FlipFlag::Diagonal)) {
    // 0010
    doDFlip();
  } else if (!(flip_type & tmx::TileLayer::FlipFlag::Horizontal) &&
             (flip_type & tmx::TileLayer::FlipFlag::Vertical) &&
             (flip_type & tmx::TileLayer::FlipFlag::Diagonal)) {
    // 0110
    doXFlip();
    doDFlip();
  } else if ((flip_type & tmx::TileLayer::FlipFlag::Horizontal) &&
             !(flip_type & tmx::TileLayer::FlipFlag::Vertical) &&
             (flip_type & tmx::TileLayer::FlipFlag::Diagonal)) {
    // 1010
    doYFlip();
    doDFlip();
  } else if ((flip_type & tmx::TileLayer::FlipFlag::Horizontal) &&
             (flip_type & tmx::TileLayer::FlipFlag::Vertical) &&
             (flip_type & tmx::TileLayer::FlipFlag::Diagonal)) {
    // 1110
    doXFlip();
    doYFlip();
    doDFlip();
  }
}
