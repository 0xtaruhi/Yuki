#include "MainScene.hpp"

#include "Basic.hpp"
#include "InteractiveObject.hpp"

using namespace yuki;
using namespace sf;

const sf::Vector2i MainScene::kOwnSoldierBirthCoordinate = {3, 17};
const sf::Vector2i MainScene::kEnemySoldierBirthCoordinate = {25, 17};

// const sf::Vector2i MainScene::kOwnBaseCoordinate = {3, 18};
const sf::Vector2i MainScene::kEnemyBaseCoordinate = {26, 14};

std::unique_ptr<Soldier> MainScene::generateSoldier(const std::string& name,
                                                    Camp camp) {
  auto soldier = getSoldier(name, camp);
  registerTouchableObject(soldier.get());

  Vector2i birth_coordinate;

  if (camp == Camp::Own) {
    birth_coordinate = kOwnSoldierBirthCoordinate;
    soldier->setElementumType(own_current_elementum_type_);
  } else {
    birth_coordinate = kEnemyBaseCoordinate;
  }
  soldier->setPosition(coordinateToPixel(birth_coordinate));

  return soldier;
}

MainScene::MainScene(sf::RenderWindow& window)
    : YukiScene(window), map_({30, 20}, {32, 32}) {
  // init ui
  initUi();
}

int MainScene::show() { return YukiScene::show(); }

void MainScene::processEvent(sf::Event event) {
  YukiScene::processEvent(event);

#ifdef YUKI_DEBUG
  if (event.type == sf::Event::MouseButtonPressed) {
    auto pixel_position =
        sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
    auto coordinate = pixelToCoordinate(pixel_position);
    debug_text_.setString("Coordinate: " + std::to_string(coordinate.x) + ", " +
                          std::to_string(coordinate.y));
  } else {
    auto pixel_position = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
    debug_text_.setString("Pixel: " + std::to_string(pixel_position.x) + ", " +
                          std::to_string(pixel_position.y));
  }
#endif
}

void MainScene::draw() const {
  window_.draw(map_);

  for (auto& soldier : soldiers_) {
    window_.draw(*soldier);
  }
  for (auto& enemy : enemies_) {
    window_.draw(*enemy);
  }

  for (auto& granule_attack : granule_attacks_) {
    window_.draw(granule_attack.second);
  }

  window_.draw(*own_base_);
  window_.draw(*enemy_base_);

  window_.draw(*info_bar_);
  window_.draw(*skill_bar_);

  for (int i = 0; i != 3; ++i) {
    window_.draw(elementum_panels_[i]);
  }

#ifdef YUKI_DEBUG
  window_.draw(debug_text_);
#endif
}

void MainScene::updateInfo() {
  for (auto& soldier : soldiers_) {
    soldier->update();
    if (soldier->getHealth() > 50.f) {
      soldier->decreaseHealth(1.f);
    }
  }
  for (auto& enemy : enemies_) {
    enemy->update();
  }

  for (auto& granule_attack : granule_attacks_) {
    granule_attack.second.update();
  }

  while (!message_queue_.empty()) {
    auto message = message_queue_.front();
    message_queue_.pop();
    switch (message) {
      case Message::GenerateOwnSoldier:
        generateSoldier();
        break;
      default:
        break;
    }
  }
  eraseDeadSoldier();
}

void MainScene::eraseDeadSoldier() {
  const auto& window_size = window_.getSize();

  auto left_bound = -map_.getTileSize().x;
  auto right_bound = window_size.x;
  auto up_bound = -map_.getTileSize().y;
  auto down_bound = window_size.y;

  auto isDead = [=](const std::unique_ptr<Soldier>& soldier) {
    if (soldier->getHealth() <= 0.f) {
      return true;
    }

    // out of window
    const auto& soldier_pos = soldier->getPosition();
    if (soldier_pos.x < left_bound || soldier_pos.x >= right_bound ||
        soldier_pos.y < up_bound || soldier_pos.y >= down_bound) {
      return true;
    }
    return false;
  };

  soldiers_.erase(std::remove_if(soldiers_.begin(), soldiers_.end(), isDead),
                  soldiers_.end());
  enemies_.erase(std::remove_if(enemies_.begin(), enemies_.end(), isDead),
                 enemies_.end());

  sodierAdjustDirection();
}

void MainScene::sodierAdjustDirection() {
  auto valid_coordinate = [=](const sf::Vector2i& coordinate) {
    return coordinate.x >= 0 && coordinate.x < map_.getSize().x &&
           coordinate.y >= 0 && coordinate.y < map_.getSize().y;
  };
  auto walkable = [=](const sf::Vector2i& coordinate) {
    if (!valid_coordinate(coordinate)) {
      return false;
    }
    return map_.getTile(coordinate).getTileInfo().category ==
           TileCategory::Road;
  };
  // auto getLeftCoordinate = [=](const sf::Vector2i& coordinate) {
  //   return sf::Vector2i(coordinate.x - 1, coordinate.y);
  // };
  auto getRightCoordinate = [=](const sf::Vector2i& coordinate) {
    return sf::Vector2i(coordinate.x + 1, coordinate.y);
  };
  auto getUpCoordinate = [=](const sf::Vector2i& coordinate) {
    return sf::Vector2i(coordinate.x, coordinate.y - 1);
  };
  auto getDownCoordinate = [=](const sf::Vector2i& coordinate) {
    return sf::Vector2i(coordinate.x, coordinate.y + 1);
  };

  auto distanceToEnemyBase = [=](const sf::Vector2i& coordinate) {
    return std::abs(coordinate.x - kEnemyBaseCoordinate.x) +
           std::abs(coordinate.y - kEnemyBaseCoordinate.y);
  };

  std::for_each(soldiers_.begin(), soldiers_.end(), [=](auto& soldier) {
    auto coordinate = pixelToCoordinate(soldier->getPosition());
    // coordinate fix (for origin at left-top)
    coordinate.x++;
    coordinate.y++;

    auto distance = distanceToEnemyBase(coordinate);
    if (distance < 2.f) {
      soldier->setMoving(false);
      return;
    }

    auto next_coordinate(coordinate);
    switch (soldier->getDirection()) {
      case Direction::Up:
        next_coordinate.y -= 1;
        break;
      case Direction::Down:
        next_coordinate.y += 1;
        break;
      case Direction::Left:
        next_coordinate.x -= 1;
        break;
      case Direction::Right:
        next_coordinate.x += 1;
        break;
      default:
        break;
    }
    if (!walkable(next_coordinate)) {
      // if (walkable(getLeftCoordinate(coordinate))) {
      //   soldier->setDirection(Direction::Left);
      // } else if (walkable(getRightCoordinate(coordinate))) {
      if (walkable(getRightCoordinate(coordinate))) {
        soldier->setDirection(Direction::Right);
      } else if (walkable(getUpCoordinate(coordinate))) {
        soldier->setDirection(Direction::Up);
      } else if (walkable(getDownCoordinate(coordinate))) {
        soldier->setDirection(Direction::Down);
      } else {
        soldier->setMoving(false);
      }
    }
  });
}

void MainScene::updateAttacks() {
  auto isValidPosition = [=](const sf::Vector2f& position) {
    return position.x >= 0 && position.x < window_.getSize().x &&
           position.y >= 0 && position.y < window_.getSize().y;
  };

  auto granuleHitEnemy =
      [=](const GranuleAttackWithSenderInfo& granule_attack) {
        bool hit = false;
        const auto& granule_attack_pos = granule_attack.second.getPosition();
        for (auto& enemy : enemies_) {
          if (enemy->inRange(granule_attack_pos)) {
            hit = true;
            break;
          }
          break;
        }
        return hit;
      };

  for (auto& granule_attack : granule_attacks_) {
    auto& granule_attack_ptr = granule_attack.second;
    granule_attack_ptr.update();
  }
  granule_attacks_.erase(
      std::remove_if(granule_attacks_.begin(), granule_attacks_.end(),
                     [=](auto& granule_attack) {
                       return !isValidPosition(
                                  granule_attack.second.getPosition()) ||
                              granuleHitEnemy(granule_attack);
                     }),
      granule_attacks_.end());
}

void MainScene::generateSoldier() {
  auto new_soldier = generateSoldier("NormalSoldier", Camp::Own);

  auto new_soldier_ptr = new_soldier.get();
  new_soldier->bindHover([=](sf::Event) {
    if (!new_soldier_ptr->isFocused()) {
      new_soldier_ptr->setColor({255, 255, 255, 192});
    }
  });
  new_soldier->bindLeave([=](sf::Event) {
    if (!new_soldier_ptr->isFocused()) {
      new_soldier_ptr->setColor({255, 255, 255, 255});
    }
  });
  new_soldier->bindClick([=](sf::Event) {
    // dynamic_cast<NormalSoldier*>(new_soldier_ptr)
    //     ->addGranuleAttack(new_soldier_ptr->getPosition(),
    //       AttackInfo(ElementumType::Hydro, 2.0, 10.0)
    //     );
    registerGranuleAttack(
        new_soldier_ptr,
        GranuleAttack({new_soldier_ptr->getPosition().x +
                           new_soldier_ptr->getSize().x / 2,
                       new_soldier_ptr->getPosition().y +
                           new_soldier_ptr->getSize().y / 2},
                      5.0f, new_soldier_ptr->getDirection(),
                      AttackInfo(new_soldier_ptr->getElementumType(), 2.0, 10.0)));
  });
  new_soldier->bindFocus([=](sf::Event) {
    new_soldier_ptr->setColor({255, 0, 200, 255});
    focused_object_ = new_soldier_ptr;
    focused_object_type_ = ObjectType::OwnSoldier;
  });
  new_soldier->bindUnfocus([=](sf::Event) {
    new_soldier_ptr->setColor({255, 255, 255, 255});
  });
  soldiers_.push_back(std::move(new_soldier));
}

void MainScene::initUi() {
  initMap();
  initBuildings();
  initInfoHint();
  initElementumPanel();
  initButton();
#ifdef YUKI_DEBUG
  debug_text_.setCharacterSize(20);
  debug_text_.setFillColor(sf::Color::Black);
  debug_text_.setPosition(0, 0);
  debug_text_.setFont(GameFont::DefaultFont);
#endif
}

void MainScene::initMap() {
  map_.setSize({30, 20});
  map_.setTileSize({32, 32});

  auto isValidCoordinate = [&](int x, int y) {
    return x >= 0 && x < map_.getSize().x && y >= 0 && y < map_.getSize().y;
  };

  auto has_tile =
      std::vector(map_.getSize().x, std::vector(map_.getSize().y, false));
  auto setTile = [&](int x, int y, TileCategory category, int index) {
    map_.setTile(x, y, TileInfo(category, index));

    if (category == TileCategory::Road || category == TileCategory::Tree ||
        category == TileCategory::River) {
      for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
          if (isValidCoordinate(x + i, y + j)) has_tile[x + i][y + j] = true;
        }
      }
    } else {
      has_tile[x][y] = true;
    }
  };

  auto getRandomCoordinate = [&]() {
    int x, y;
    do {
      x = rand() % map_.getSize().x;
      y = rand() % map_.getSize().y;
    } while (has_tile[x][y]);
    return Vector2i(x, y);
  };

  // road
  for (int i = 7; i != 20; ++i) {
    setTile(3, i, TileCategory::Road, 4);
    setTile(4, i, TileCategory::Road, 6);
    setTile(25, i, TileCategory::Road, 4);
    setTile(26, i, TileCategory::Road, 6);
  }
  setTile(3, 6, TileCategory::Road, 1);
  setTile(26, 6, TileCategory::Road, 3);
  for (int i = 4; i != 10; ++i) {
    setTile(i, 6, TileCategory::Road, 2);
  }
  for (int i = 5; i != 10; ++i) {
    setTile(i, 7, TileCategory::Road, 8);
  }
  setTile(10, 6, TileCategory::Road, 3);
  setTile(9, 7, TileCategory::Road, 11);
  for (int i = 7; i != 18; ++i) {
    setTile(9, i + 1, TileCategory::Road, 4);
    setTile(10, i, TileCategory::Road, 6);
  }
  setTile(10, 18, TileCategory::Road, 12);
  setTile(9, 19, TileCategory::Road, 7);
  for (int i = 10; i != 17; ++i) {
    setTile(i, 19, TileCategory::Road, 8);
  }
  for (int i = 11; i != 16; ++i) {
    setTile(i, 18, TileCategory::Road, 2);
  }
  setTile(16, 18, TileCategory::Road, 13);
  setTile(17, 19, TileCategory::Road, 9);
  for (int i = 11; i != 18; ++i) {
    setTile(16, i, TileCategory::Road, 4);
    setTile(17, i + 1, TileCategory::Road, 6);
  }
  setTile(16, 10, TileCategory::Road, 1);
  setTile(17, 11, TileCategory::Road, 10);
  for (int i = 17; i != 20; ++i) {
    setTile(i, 10, TileCategory::Road, 2);
    setTile(i + 1, 11, TileCategory::Road, 8);
  }
  setTile(20, 10, TileCategory::Road, 13);
  setTile(21, 11, TileCategory::Road, 9);
  for (int i = 7; i != 10; ++i) {
    setTile(20, i, TileCategory::Road, 4);
    setTile(21, i + 1, TileCategory::Road, 6);
  }
  setTile(20, 6, TileCategory::Road, 1);
  setTile(21, 7, TileCategory::Road, 10);
  for (int i = 21; i != 26; ++i) {
    setTile(i, 6, TileCategory::Road, 2);
  }
  for (int i = 22; i != 25; ++i) {
    setTile(i, 7, TileCategory::Road, 8);
  }
  setTile(4, 7, TileCategory::Road, 10);
  setTile(25, 7, TileCategory::Road, 11);

  // river
  for (int i = 0; i != 10; ++i) {
    setTile(i, 2, TileCategory::River, 1);
  }
  setTile(11, 2, TileCategory::River, 4);
  for (int i = 4; i != 7; ++i) {
    setTile(11, i, TileCategory::River, 5);
  }
  setTile(11, 8, TileCategory::River, 3);
  for (int i = 13; i != 17; ++i) {
    setTile(i, 8, TileCategory::River, 1);
  }
  setTile(18, 8, TileCategory::River, 2);
  for (int i = 7; i != -1; --i) {
    setTile(18, i, TileCategory::River, 5);
  }

  // trees
  srand(0x35d5d);
  for (int i = 0; i != 30; ++i) {
    auto rand_cor = getRandomCoordinate();
    setTile(rand_cor.x, rand_cor.y, TileCategory::Tree, 1 + rand() % 15);
  }

  // greenery
  for (int i = 0; i != 30; ++i) {
    auto rand_cor = getRandomCoordinate();
    setTile(rand_cor.x, rand_cor.y, TileCategory::Greenery, 1 + rand() % 9);
  }

  // decor
  for (int i = 0; i != 8; ++i) {
    auto rand_cor = getRandomCoordinate();
    setTile(rand_cor.x, rand_cor.y, TileCategory::Decor, 1 + rand() % 11);
  }
}

void MainScene::initBuildings() {
  own_base_ = std::make_unique<MilitaryBase>(Camp::Own);
  enemy_base_ = std::make_unique<MilitaryBase>(Camp::Enemy);

  own_base_->setPosition(coordinateToPixel({2, 14}));
  enemy_base_->setPosition(coordinateToPixel({24, 14}));
  registerTouchableObject(own_base_.get());
  // registerTouchableObject(std::shared_ptr<Touchable>(&enemy_base_));

  // floating bubble init
  auto& own_base_fb = own_base_->getFloatingBubble();
  auto& own_base_fb_items = own_base_fb.getBubbleItems();

  own_base_fb_items[0]->loadTexture("assets/res/update_base.jpg");

  for (const auto& item : own_base_fb_items) {
    auto item_ptr = item.get();
    item->bindHover([=](sf::Event) {
      // set semi-transparent
      item_ptr->setFillColor(YukiColor::Normal);
    });
    item->bindLeave([=](sf::Event) {
      // set opaque
      item_ptr->setFillColor(YukiColor::Transparent_25);
    });

    registerTouchableObject(item_ptr);
  }

  // Bind Floating Bubble Actions
  own_base_->bindClick([=](sf::Event) {
    const auto& mouse_pos = sf::Mouse::getPosition(window_);
    auto bubble_index = own_base_->getFloatingBubbleIndexByPosition(
        Vector2f(mouse_pos.x, mouse_pos.y));
    if (bubble_index == -1) return;
    if (bubble_index == 2) {
      sendMessage(Message::GenerateOwnSoldier);
    }
  });
}

void MainScene::initInfoHint() {
  // info bar
  info_bar_ = std::make_unique<InfoBar>();

  info_bar_->setSize(Vector2f{168.f, 32.f});
  info_bar_->setPosition(
      Vector2f(10.f, window_.getSize().y - info_bar_->getSize().y - 10.f));
  setMoney(150);

  // skill bar
  skill_bar_ = std::make_unique<SkillBar<3>>();
  registerTouchableObject(skill_bar_.get());
}

void MainScene::initElementumPanel() {
  for (int i = 0; i != 3; ++i) {
    auto& panel = elementum_panels_[i];

    panel.setRadius(20.f);
    panel.setPosition(Vector2f(window_.getSize().x - 50.f * (i + 1), 25.f));

    panel.bindClick([&](sf::Event) {
      setOwnCurrentElementumType(panel.getElementumType());
    });

    registerTouchableObject(&panel);
  }
  elementum_panels_[0].setElementumType(ElementumType::Pyro);
  elementum_panels_[1].setElementumType(ElementumType::Hydro);
  elementum_panels_[2].setElementumType(ElementumType::Cyro);

  setOwnCurrentElementumType(ElementumType::Cyro);
}

void MainScene::setMoney(int money) {
  money_ = money;
  info_bar_->setMoney(money);
}

sf::Vector2f MainScene::coordinateToPixel(const Vector2i& coordinate) {
  auto tile_size = map_.getTileSize();
  return {coordinate.x * tile_size.x, coordinate.y * tile_size.y};
}

sf::Vector2i MainScene::pixelToCoordinate(const sf::Vector2f& pixel_position) {
  auto window_size = window_.getSize();
  if (pixel_position.x < 0 || pixel_position.x >= window_size.x ||
      pixel_position.y < 0 || pixel_position.y >= window_size.y) {
    return {-1, -1};
  }

  auto tile_size = map_.getTileSize();
  return {static_cast<int>(pixel_position.x / tile_size.x),
          static_cast<int>(pixel_position.y / tile_size.y)};
}

void MainScene::setOwnCurrentElementumType(ElementumType type) {
  own_current_elementum_type_ = type;
  for (auto& panel : elementum_panels_) {
    if (panel.getElementumType() == type) {
      panel.activate();
    } else {
      panel.deactivate();
    }
  }
}

void MainScene::initButton() {
  attack_button_.setRadius(30.f);
  attack_button_.bindClick([&](Event) {
    if (focused_object_type_ == ObjectType::OwnSoldier) {
      // sendMessage(Message::Attack);
    }
  });

}
