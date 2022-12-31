#include "Menu.hpp"

using namespace yuki;

Menu::Menu() {}

Menu::~Menu() {}

void Menu::addItem(std::unique_ptr<Button> item) {
  auto items_num = menu_items_.size();
  if (items_num > 0) {
    auto& last_item = menu_items_.back();
    auto last_item_size = last_item->getSize();
    auto last_item_position = last_item->getPosition();
    if (layout_ == Layout::Vertical) {
      item->setPosition(last_item_position.x,
                        last_item_position.y + last_item_size.y + spacing_);
    } else {
      item->setPosition(last_item_position.x + last_item_size.x + spacing_,
                        last_item_position.y);
    }
  } else {
    item->setPosition(position_);
  }

  menu_items_.push_back(std::move(item));
}

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  for (auto& item : menu_items_) {
    target.draw(*item, states);
  }
}

void Menu::adjustItemsPosition() {
  
}