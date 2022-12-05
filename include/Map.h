#include <SFML/Graphics.hpp>
#include <string>
#include <tmxlite/Map.hpp>
#include <vector>

#include <iostream>

#include "YukiBasic.h"
#include "tmxlite/Layer.hpp"
#include "tmxlite/TileLayer.hpp"

namespace yuki {

class Map {
 public:
  Map();
  Map(const std::string& map_path, WindowHandler window);

  void draw() const;
  void setScale(const sf::Vector2f& scale);

  const auto getPixelSize() const {
    return sf::Vector2f(map_.getTileSize().x * map_.getTileCount().x,
                        map_.getTileSize().y * map_.getTileCount().y);
  }

  const auto getTileSize() const { return map_.getTileSize(); }
  void adjustScaleToWindow() {
    const auto window_size = parent_window_->getSize();
    const auto pixel_size = getPixelSize();
    const auto scale_x = window_size.x / pixel_size.x;
    const auto scale_y = window_size.y / pixel_size.y;
    std::cout << window_size.x << " " << window_size.y << std::endl;
    std::cout << pixel_size.x << " " << pixel_size.y << std::endl;
    setScale(sf::Vector2f(scale_x, scale_y));
    std::cout << scale_.x << " " << scale_.y << std::endl;
  }

 private:
  WindowHandler parent_window_;
  tmx::Map map_;
  std::vector<std::unique_ptr<sf::Texture>> textures_;
  tmx::Vector2u tile_size_;

  sf::Vector2f scale_ = sf::Vector2f(1, 1);

  sf::RectangleShape getShapeByTile(const tmx::TileLayer::Tile& tile) const;

  void doFlip(const tmx::TileLayer::Tile& tile,
              sf::RectangleShape& shape) const;
};

};  // namespace yuki
