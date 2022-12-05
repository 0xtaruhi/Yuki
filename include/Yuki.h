#ifndef YUKI_H
#define YUKI_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <tmxlite/Map.hpp>

#include "Map.h"
#include "YukiBasic.h"
#include "Soldier.h"

namespace yuki {

class Yuki {
 public:
  Yuki(int window_width, int window_height);
  Yuki();
  virtual ~Yuki();

  void run();
  void stop();

  void update();

 private:
  WindowHandler window_;
  sf::Event event_;
  Map map_;
  NormalSoldier test_soldier_;
};

};  // namespace yuki

#endif
