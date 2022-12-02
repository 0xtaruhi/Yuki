#include <SFML/Graphics.hpp>
#include <string>
#include <tmxlite/Map.hpp>
#include <vector>

#include "SFMLOrthogonalLayer.h"
#include "YukiBasic.h"

namespace yuki {

class Map {
 public:
  Map();
  Map(const std::string& map_path, WindowHandler window);

  void draw() const;

 private:
  WindowHandler parent_window_;
  tmx::Map map_;
  // std::vector<MapLayer> layers_;
  std::vector<std::unique_ptr<MapLayer>> layers_;
};

};  // namespace yuki
