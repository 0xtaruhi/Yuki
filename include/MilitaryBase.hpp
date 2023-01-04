#ifndef MILITARY_BASE_HPP
#define MILITARY_BASE_HPP

#include "TouchableSprite.hpp"

namespace yuki {

class MilitaryBase : public TouchableSprite {
  TOUCHABLE_OBJECT(MilitaryBase)

 public:
  MilitaryBase();
  void levelUp() { level++; }
  void levelDown() { level--; }

  virtual ~MilitaryBase() {}

 private:
  int level = 1;
  sf::Texture texture_;

  void initTexture();
  void updateTexture();
};

}  // namespace yuki

#endif
