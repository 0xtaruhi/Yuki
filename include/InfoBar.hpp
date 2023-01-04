#ifndef INFO_BAR_HPP
#define INFO_BAR_HPP

#include <SFML/Graphics.hpp>

namespace yuki {

class InfoBar : public sf::Drawable {
 private:
  constexpr static float kIconSize = 32.f;
  constexpr static float kMoneyIconOffset = 10.f;
  constexpr static float kMoneyTextOffset = 60.f;

 public:
  InfoBar() : InfoBar(sf::Vector2f(0.f, 0.f)) {}
  InfoBar(const sf::Vector2f& position,
          const sf::Vector2f& size = sf::Vector2f(256.f, 32.f));
  virtual ~InfoBar() = default;

  void setMoney(const int money);
  void setPosition(const sf::Vector2f& position);
  const auto& getPosition() const { return background_.getPosition(); }

  void setSize(const sf::Vector2f& size) {
    background_.setSize(size);
  }
  const auto& getSize() const { return background_.getSize(); }


 private:
  sf::RectangleShape background_;
  sf::Texture background_texture_;
  sf::Sprite icon_money_;
  sf::Texture icon_money_texture_;
  sf::Text text_money_;

  void initTextures();
  void initUi();
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

}  // namespace yuki

#endif  // INFO_BAR_HPP
