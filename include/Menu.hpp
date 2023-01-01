#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Button.hpp"

namespace yuki {

class Menu : public sf::Drawable {
 public:
  enum class Layout {
    Vertical,
    Horizontal,
  };

  Menu();

  virtual ~Menu();

  void setLayout(Layout layout) { layout_ = layout; }
  const Layout& getLayout() const { return layout_; }

  void addItem(std::unique_ptr<yuki::Button> item);

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

  Button& operator[](unsigned int index) { return *menu_items_[index]; }
  Button& getByName(const sf::String& name) {
    for (auto& item : menu_items_) {
      if (item->getString() == name) {
        return *item;
      }
    }
    throw std::runtime_error("Menu item not found");
    return *menu_items_[0];
  }

 private:
  std::vector<std::unique_ptr<yuki::Button>> menu_items_;
  Layout layout_;
  unsigned int spacing_;

  sf::Vector2f position_;

  void adjustItemsPosition();
};

}  // namespace yuki

#endif
