#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

namespace yuki {

enum class TileCategory {
  Building,
  Decor,
  Greenery,
  Land,
  River,
  Road,
  Stones,
  Tree
};

struct TileInfo {
  TileCategory category;
  int index;

  TileInfo() = default;
  TileInfo(TileCategory category, int index)
      : category(category), index(index) {}
};

inline std::string getTileCategoryName(const TileCategory category);
inline std::string getTileSrcName(const TileInfo& tile_info);
inline sf::Vector2f getTileScaleFactor(const TileInfo& tile_info);

class Tile : public sf::Sprite {
 public:
  auto getTileInfo() const { return tile_info_; }
  void setTileInfo(TileInfo&& tile_info) { tile_info_ = tile_info; }

  void setTextureSrc(const std::string& src_location) {
    texture_.loadFromFile(src_location);
    this->setTexture(texture_);
  }

 private:
  TileInfo tile_info_;
  sf::Texture texture_;
};

class Map : public sf::Drawable {
 public:
  Map(const sf::Vector2i& size, const sf::Vector2f& tile_size);
  virtual ~Map();

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  void setSize(const sf::Vector2i& size) {
    size_ = size;
  }
  const auto& getSize() const { return size_; }
  void setTileSize(const sf::Vector2f& tile_size) {
    tile_size_ = tile_size;
  }
  const auto& getTileSize() const { return tile_size_; }

  auto getCoordinate(const sf::Vector2f& position) const {
    return sf::Vector2i(position.x / tile_size_.x, position.y / tile_size_.y);
  }

  Tile& getTile(int row, int col);
  const Tile& getTile(int row, int col) const;

  void setTile(int row, int col, const TileInfo& tile_info);

 private:
  sf::Vector2i size_;
  sf::Vector2f tile_size_;

  std::vector<Tile> tiles_;
  
  sf::Texture bg_texture_;
  std::vector<sf::Sprite> bg_tiles_;
};

}  // namespace yuki

#endif
