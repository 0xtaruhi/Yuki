#include <SFML/Graphics.hpp>
#include <string>
#include <tmxlite/Map.hpp>
#include <vector>

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

 private:
  WindowHandler parent_window_;
  tmx::Map map_;
  std::vector<std::unique_ptr<sf::Texture>> textures_;
  tmx::Vector2u tile_size_;

  sf::Vector2f scale_ = sf::Vector2f(1, 1);
  
  sf::RectangleShape getShapeByTile(const tmx::TileLayer::Tile& tile) const;

  void doFlip(const tmx::TileLayer::Tile& tile, sf::RectangleShape& shape) const;
};

};  // namespace yuki
