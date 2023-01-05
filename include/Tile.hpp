#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>
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

std::string getTileCategoryName(const TileCategory category);
std::string getTileSrcName(const TileInfo& tile_info);
sf::Vector2f getTileScaleFactor(const TileInfo& tile_info);

class Tile : public sf::Sprite {
 public:
  auto getTileInfo() const { return tile_info_; }
  void setTileInfo(TileInfo&& tile_info) { tile_info_ = std::move(tile_info); }
  void setTileInfo(const TileInfo& tile_info) { tile_info_ = tile_info; }

  void setTextureSrc(const std::string& src_location) {
    texture_.loadFromFile(src_location);
    this->setTexture(texture_);
  }

 private:
  TileInfo tile_info_;
  sf::Texture texture_;
};

} // namespace yuki

#endif // TILE_HPP
