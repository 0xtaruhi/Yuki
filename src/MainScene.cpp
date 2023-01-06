#include "MainScene.hpp"

#include "Basic.hpp"
#include "InteractiveObject.hpp"

using namespace yuki;
using namespace sf;

const sf::Vector2i MainScene::kOwnSoldierBirthCoordinate = {3, 17};
const sf::Vector2i MainScene::kEnemySoldierBirthCoordinate = {25, 17};

const sf::Vector2i MainScene::kOwnBaseCoordinate = {3, 14};
const sf::Vector2i MainScene::kEnemyBaseCoordinate = {26, 14};

std::unique_ptr<Soldier> MainScene::generateSoldier(const std::string& name,
                                                    Camp camp) {
  auto soldier = getSoldier(name, camp);
  if (camp == Camp::Own) registerTouchableObject(soldier.get());

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
  window_.draw(attack_button_);

#ifdef YUKI_DEBUG
  window_.draw(debug_text_);
#endif
}

void MainScene::updateInfo() {
  enemyBaseMakeDecision();
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
    if (message == Message::GenerateOwnSoldier) {
      implMsgGenerateOwnSoldier();
    } else if (message == Message::GenerateEnemySoldier) {
      implMsgGenerateEnemySoldier();
    } else if (message == Message::OwnBaseLevelUp) {
      if (own_base_->getLevel() < 3) {
        own_base_->levelUp();
      }
    }
  }
  updateAttacks();
  soldierUpdatePosition();
  eraseDeadSoldier();
}

void MainScene::eraseDeadSoldier() {
  const auto& window_size = window_.getSize();

  const auto left_bound = -map_.getTileSize().x;
  const auto right_bound = window_size.x;
  const auto up_bound = -map_.getTileSize().y;
  const auto down_bound = window_size.y;

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

  // erase & unregister
  for (auto iter = soldiers_.begin(); iter != soldiers_.end();) {
    if (isDead(*iter)) {
      auto ptr = iter->get();
      unregisterTouchableObject(ptr);
      iter = soldiers_.erase(iter);
      increaseEnemyMoney(75);
    } else {
      ++iter;
    }
  }

  for (auto iter = enemies_.begin(); iter != enemies_.end();) {
    if (isDead(*iter)) {
      iter = enemies_.erase(iter);
      increaseOwnMoney(75);
    } else {
      ++iter;
    }
  }
}

void MainScene::soldierUpdatePosition() {
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
  auto getLeftCoordinate = [=](const sf::Vector2i& coordinate) {
    return sf::Vector2i(coordinate.x - 1, coordinate.y);
  };
  auto getRightCoordinate = [=](const sf::Vector2i& coordinate) {
    return sf::Vector2i(coordinate.x + 1, coordinate.y);
  };
  auto getUpCoordinate = [=](const sf::Vector2i& coordinate) {
    return sf::Vector2i(coordinate.x, coordinate.y - 1);
  };
  auto getDownCoordinate = [=](const sf::Vector2i& coordinate) {
    return sf::Vector2i(coordinate.x, coordinate.y + 1);
  };

  auto distanceToEnemyBase = [=](Soldier* soldier) {
    auto camp = soldier->getCamp();
    auto coordinate = pixelToCoordinate(soldier->getPosition());
    if (camp == Camp::Own) {
      return std::abs(coordinate.x - kEnemyBaseCoordinate.x) +
             std::abs(coordinate.y - kEnemyBaseCoordinate.y);
    }
    return std::abs(coordinate.x - kOwnBaseCoordinate.x) +
           std::abs(coordinate.y - kOwnBaseCoordinate.y);
  };

  auto distanceBetweenSoldier = [=](const Soldier& soldier_A,
                                    const Soldier& soldier_B) {
    const auto& coordinate_A = pixelToCoordinate(soldier_A.getPosition());
    const auto& coordinate_B = pixelToCoordinate(soldier_B.getPosition());
    return std::abs(coordinate_A.x - coordinate_B.x) +
           std::abs(coordinate_A.y - coordinate_B.y);
  };

  auto turnDirection = [=](Soldier* soldier) {
    auto camp = soldier->getCamp();
    auto coordinate = pixelToCoordinate(soldier->getPosition());
    coordinate.x++;
    coordinate.y++;

    if (camp == Camp::Own) {
      if (walkable(getRightCoordinate(coordinate)))
        soldier->setDirection(Direction::Right);
      else if (walkable(getUpCoordinate(coordinate)))
        soldier->setDirection(Direction::Up);
      else if (walkable(getDownCoordinate(coordinate)))
        soldier->setDirection(Direction::Down);
      else
        soldier->setSpeed(0.f);
    } else {
      if (walkable(getLeftCoordinate(coordinate)))
        soldier->setDirection(Direction::Left);
      else if (walkable(getUpCoordinate(coordinate)))
        soldier->setDirection(Direction::Up);
      else if (walkable(getDownCoordinate(coordinate)))
        soldier->setDirection(Direction::Down);
      else
        soldier->setSpeed(0.f);
    }
  };

  auto updateSoldierPostionByCamp = [&](Camp camp) {
    std::vector<std::unique_ptr<Soldier>>* soldiers_ptr;
    std::vector<std::unique_ptr<Soldier>>* enemies_ptr;

    if (camp == Camp::Own) {
      soldiers_ptr = &soldiers_;
      enemies_ptr = &enemies_;
    } else {
      soldiers_ptr = &enemies_;
      enemies_ptr = &soldiers_;
    }
    auto& soldiers = *soldiers_ptr;
    auto& enemies = *enemies_ptr;

    std::for_each(soldiers.begin(), soldiers.end(), [&](auto& soldier) {
      auto coordinate = pixelToCoordinate(soldier->getPosition());
      coordinate.x++;
      coordinate.y++;

      if (!soldier->isFreezed()) {
        soldier->setMoving(true);
      } else {
        soldier->setMoving(false);
      }

      auto distance = distanceToEnemyBase(soldier.get());
      if (distance < 2.f) {
        soldier->setMoving(false);
        return;
      }

      for (auto iter = enemies.begin(); iter != enemies.end(); iter++) {
        auto& enemy = *iter;
        if (isOppositeDirection(soldier->getDirection(),
                                enemy->getDirection()) &&
            distanceBetweenSoldier(*soldier, *enemy) < 3.f) {
          soldier->setMoving(false);
          return;
        }
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
        turnDirection(soldier.get());
        return;
      }
    });
  };

  updateSoldierPostionByCamp(Camp::Own);
  updateSoldierPostionByCamp(Camp::Enemy);
}

void MainScene::updateAttacks() {
  auto isValidPosition = [=](const sf::Vector2f& position) {
    return position.x >= 0 && position.x < window_.getSize().x &&
           position.y >= 0 && position.y < window_.getSize().y;
  };

  auto granuleHitEnemy =
      [=](const GranuleAttackWithSenderInfo& granule_attack) -> Soldier* {
    const auto& granule_attack_pos = granule_attack.second.getPosition();
    const auto& attack_sender_camp = granule_attack.first->getCamp();
    if (attack_sender_camp == Camp::Own) {
      for (auto& enemy : enemies_) {
        if (enemy->inRange(granule_attack_pos)) {
          return enemy.get();
          break;
        }
      }
    } else if (attack_sender_camp == Camp::Enemy) {
      for (auto& soldier : soldiers_) {
        if (soldier->inRange(granule_attack_pos)) {
          return soldier.get();
          break;
        }
      }
    }
    return nullptr;
  };

  auto granuleHitBase = [=](const GranuleAttackWithSenderInfo& granule_attack) -> MilitaryBase* {
    const auto& granule_attack_pos = granule_attack.second.getPosition();
    const auto& attack_sender_camp = granule_attack.first->getCamp();
    if (attack_sender_camp == Camp::Own) {
      if (enemy_base_->inRange(granule_attack_pos)) {
        return enemy_base_.get();
      }
    } else if (attack_sender_camp == Camp::Enemy) {
      if (own_base_->inRange(granule_attack_pos)) {
        return own_base_.get();
      }
    }
    return nullptr;
  };

  for (auto& granule_attack : granule_attacks_) {
    auto& granule_attack_ptr = granule_attack.second;
    granule_attack_ptr.update();
  }
  granule_attacks_.erase(
      std::remove_if(granule_attacks_.begin(), granule_attacks_.end(),
                     [=](auto& granule_attack) {
                       return !isValidPosition(
                           granule_attack.second.getPosition());
                     }),
      granule_attacks_.end());

  for (auto iter = granule_attacks_.begin(); iter != granule_attacks_.end();) {
    auto hit_enemy = granuleHitEnemy(*iter);
    if (hit_enemy != nullptr) {
      hit_enemy->getAttacked(iter->second);
      iter = granule_attacks_.erase(iter);
    } else {
      auto hit_base = granuleHitBase(*iter);
      if (hit_base != nullptr) {
        hit_base->getAttacked(iter->second);
        iter = granule_attacks_.erase(iter);
      } else {
        iter++;
      }
    }
  }

}

void MainScene::enemyBaseMakeDecision() {
  if (enemies_.size() < 10) {
    sendMessage(Message::GenerateEnemySoldier);
  }
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
    if (bubble_index == 0) {
      sendMessage(Message::OwnBaseLevelUp);
    }
    if (bubble_index == 1) {
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
  setMoney(150, Camp::Own);
  setMoney(150, Camp::Enemy);

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

void MainScene::setMoney(int money, Camp camp) {
  if (camp == Camp::Own) {
    own_money_ = money;
    info_bar_->setMoney(money);
  } else {
    enemy_money_ = money;
  }
}

void MainScene::increaseMoney(int money, Camp camp) {
  if (camp == Camp::Own) {
    own_money_ += money;
    info_bar_->setMoney(own_money_);
  } else {
    enemy_money_ += money;
  }
}

void MainScene::decreaseMoney(int money, Camp camp) {
  if (camp == Camp::Own) {
    own_money_ -= money;
    info_bar_->setMoney(own_money_);
  } else {
    enemy_money_ -= money;
  }
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
  attack_button_.setPosition({230, 560});
  attack_button_.setOutlineColor(sf::Color::White);
  // attack_button_.setOutlineThickness(2.f);
  attack_button_texture_.loadFromFile("assets/res/attack_button.png");
  attack_button_texture_.setSmooth(true);
  attack_button_.setTexture(&attack_button_texture_);
  attack_button_.setFillColor(YukiColor::Transparent_50);

  registerTouchableObject(&attack_button_);
  attack_button_.bindLeave(
      [&](Event) { attack_button_.setFillColor(YukiColor::Transparent_50); });
  attack_button_.bindHover(
      [&](Event) { attack_button_.setFillColor(YukiColor::Normal); });

  attack_button_.bindClick([&](Event) {
    if (focused_object_type_ == ObjectType::OwnSoldier) {
      Soldier* focused_soldier = dynamic_cast<Soldier*>(focused_object_);
      registerGranuleAttack(
          focused_soldier,
          GranuleAttack(
              {focused_soldier->getPosition().x +
                   focused_soldier->getSize().x / 2,
               focused_soldier->getPosition().y +
                   focused_soldier->getSize().y / 2},
              5.0f, focused_soldier->getDirection(),
              AttackInfo(focused_soldier->getElementumType(), 2.0, 10.0)));
    }
  });
}

void MainScene::implMsgGenerateOwnSoldier() {
  if (!(own_base_->canGenerateSoldier() && own_money_ >= 50)) return;

  own_base_->resetGenerateSoldierClock();
  decreaseOwnMoney(50);

  auto new_soldier = generateSoldier("NormalSoldier", Camp::Own);
  new_soldier->setElementumType(own_current_elementum_type_);
  new_soldier->setPosition(coordinateToPixel(kOwnSoldierBirthCoordinate));
  auto ptr = new_soldier.get();

  new_soldier->bindHover([=](sf::Event) {
    if (!ptr->isFocused()) {
      ptr->setColor({255, 255, 255, 192});
    }
  });
  new_soldier->bindLeave([=](sf::Event) {
    if (!ptr->isFocused()) {
      ptr->setColor({255, 255, 255, 255});
    }
  });
  new_soldier->bindClick([=](sf::Event) {
    registerGranuleAttack(
        ptr, GranuleAttack({ptr->getPosition().x + ptr->getSize().x / 2,
                            ptr->getPosition().y + ptr->getSize().y / 2},
                           5.0f, ptr->getDirection(),
                           AttackInfo(ptr->getElementumType(), 2.0, 2.0)));
  });
  new_soldier->bindFocus([=](sf::Event) {
    auto color = ptr->getColor();
    color.a = 192;
    ptr->setColor(color);
    focused_object_ = ptr;
    focused_object_type_ = ObjectType::OwnSoldier;
  });
  new_soldier->bindUnfocus([=](sf::Event) {
    auto color = ptr->getColor();
    color.a = 255;
    ptr->setColor(color);
  });

  soldiers_.push_back(std::move(new_soldier));
}

void MainScene::implMsgGenerateEnemySoldier() {
  if (!(enemy_base_->canGenerateSoldier() && enemy_money_ >= 50.f)) {
    return;
  }
  enemy_base_->resetGenerateSoldierClock();
  decreaseEnemyMoney(50);

  auto new_soldier = generateSoldier("NormalSoldier", Camp::Enemy);
  new_soldier->setElementumType(own_current_elementum_type_);
  new_soldier->setPosition(coordinateToPixel(kEnemySoldierBirthCoordinate));
  enemies_.push_back(std::move(new_soldier));
}
