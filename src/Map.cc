#include "Map.h"

#include <memory>

#include "SFMLOrthogonalLayer.h"

using namespace yuki;

Map::Map(const std::string& map_path, WindowHandler window)
    : parent_window_(window) {
  map_.load(map_path);
  layers_.push_back(std::make_unique<MapLayer>(map_, 0));
}

void Map::draw() const {
  for (const auto& layer : layers_) {
    parent_window_->draw(*layer);
  }
}
