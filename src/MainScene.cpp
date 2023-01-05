#include "MainScene.hpp"

#include "Basic.hpp"
#include "InteractiveObject.hpp"

using namespace yuki;
using namespace sf;

const sf::Vector2i MainScene::kOwnSodierBirthCoordinate = {3, 17};
const sf::Vector2i MainScene::kEnemySodierBirthCoordinate = {25, 17};

std::shared_ptr<Sodier> MainScene::getDefaultSodier(Camp camp) {
  auto sodier = std::make_shared<NormalSodier>(camp);
  registerTouchableObject(sodier);

  auto birth_coordinate = camp == Camp::Own ? kOwnSodierBirthCoordinate
                                            : kEnemySodierBirthCoordinate;
  sodier->setPosition(coordinateToPixel(birth_coordinate));

  sodier->setSpeed(1.f);
  sodier->setDirection(Direction::Up);

  sodier->setMoving(true);
  sodier->setMaxHealth(100.f);
  sodier->setHealth(100.f);
  return sodier;
}

MainScene::MainScene(sf::RenderWindow& window)
    : YukiScene(window), map_({30, 20}, {32, 32}) {
  // init ui
  initUi();
}

int MainScene::show() { return YukiScene::show(); }

void MainScene::processEvent(sf::Event event) {
  YukiScene::processEvent(event);
  if (event.type == sf::Event::KeyPressed) {
    if (focused_object_type_ == ObjectType::OwnSodier) {
      auto sodier = std::dynamic_pointer_cast<Sodier>(focused_object_);
      if (event.key.code == sf::Keyboard::W) {
        sodier->setDirection(Direction::Up);
      } else if (event.key.code == sf::Keyboard::S) {
        sodier->setDirection(Direction::Down);
      } else if (event.key.code == sf::Keyboard::A) {
        sodier->setDirection(Direction::Left);
      } else if (event.key.code == sf::Keyboard::D) {
        sodier->setDirection(Direction::Right);
      } else if (event.key.code == sf::Keyboard::Space) {
        sodier->setMoving(!sodier->isMoving());
      }
    }
  }

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
  window_.draw(own_base_);
  window_.draw(enemy_base_);

  window_.draw(info_bar_);

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

  while (!message_quene_.empty()) {
    auto message = message_quene_.front();
    message_quene_.pop();
    switch (message) {
      case Message::GenerateOwnSodier:
        generateSodier();
        break;
      default:
        break;
    }
  }
  eraseDeadSodier();
}

void MainScene::eraseDeadSodier() {
  const auto& window_size = window_.getSize();

  auto left_bound = -map_.getTileSize().x;
  auto right_bound = window_size.x;
  auto up_bound = -map_.getTileSize().y;
  auto down_bound = window_size.y;

  auto isDead = [=](const std::shared_ptr<Sodier>& sodier) {
    if (sodier->getHealth() <= 0.f) {
      return true;
    }

    // out of window
    const auto& sodier_pos = sodier->getPosition();
    if (sodier_pos.x < left_bound || sodier_pos.x >= right_bound ||
        sodier_pos.y < up_bound || sodier_pos.y >= down_bound) {
      return true;
    }
    return false;
  };

  soldiers_.erase(std::remove_if(soldiers_.begin(), soldiers_.end(), isDead),
                  soldiers_.end());
  enemies_.erase(std::remove_if(enemies_.begin(), enemies_.end(), isDead),
                 enemies_.end());
}

void MainScene::generateSodier() {
  auto new_sodier = getDefaultSodier();
  new_sodier->bindHover([=](sf::Event) {
    if (!new_sodier->isFocused()) {
      new_sodier->setColor({255, 255, 255, 192});
    }
  });
  new_sodier->bindLeave([=](sf::Event) {
    if (!new_sodier->isFocused()) {
      new_sodier->setColor({255, 255, 255, 255});
    }
  });
  new_sodier->bindClick([=](sf::Event) {});
  new_sodier->bindFocus([=](sf::Event) {
    new_sodier->setColor({255, 0, 200, 255});
    focused_object_ = new_sodier;
    focused_object_type_ = ObjectType::OwnSodier;
  });
  new_sodier->bindUnfocus([=](sf::Event) {
    new_sodier->setColor({255, 255, 255, 255});
  });
  soldiers_.push_back(std::move(new_sodier));
}

void MainScene::initUi() {
  initMap();
  initBuildings();
  initInfoHint();
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

    if (category == TileCategory::Road || category == TileCategory::Tree) {
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
  for (int i = 4; i != 26; ++i) {
    setTile(i, 6, TileCategory::Road, 2);
  }
  for (int i = 5; i != 25; ++i) {
    setTile(i, 7, TileCategory::Road, 8);
  }
  setTile(4, 7, TileCategory::Road, 10);
  setTile(25, 7, TileCategory::Road, 11);

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
  own_base_.setPosition(coordinateToPixel({2, 14}));
  enemy_base_.setPosition(coordinateToPixel({24, 14}));
  registerTouchableObject(std::shared_ptr<Touchable>(&own_base_));
  registerTouchableObject(std::shared_ptr<Touchable>(&enemy_base_));

  // floating bubble init
  auto& own_base_fb = own_base_.getFloatingBubble();
  auto& own_base_fb_items = own_base_fb.getBubbleItems();

  own_base_fb_items[0]->loadTexture("assets/res/update_base.jpg");

  for (const auto& item : own_base_fb_items) {
    item->bindHover([=](sf::Event) {
      // set semi-transparent
      item->setFillColor(YukiColor::Normal);
    });
    item->bindLeave([=](sf::Event) {
      // set opaque
      item->setFillColor(YukiColor::Transparent_25);
    });

    registerTouchableObject(item);
  }

  // Bind Floating Bubble Actions
  own_base_.bindClick([=](sf::Event) {
    const auto& mouse_pos = sf::Mouse::getPosition(window_);
    auto bubble_index = own_base_.getFloatingBubbleIndexByPosition(
        Vector2f(mouse_pos.x, mouse_pos.y));
    if (bubble_index == -1) return;
    if (bubble_index == 2) {
      sendMessage(Message::GenerateOwnSodier);
    }
  });
}

void MainScene::initInfoHint() {
  info_bar_.setSize(Vector2f{168.f, 32.f});
  info_bar_.setPosition(
      Vector2f(10.f, window_.getSize().y - info_bar_.getSize().y - 10.f));
  setMoney(150);
}

void MainScene::setMoney(int money) {
  money_ = money;
  info_bar_.setMoney(money);
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
