#include "MainScene.hpp"

#include "Basic.hpp"

using namespace yuki;
using namespace sf;

std::shared_ptr<Sodier> MainScene::getDefaultSodier() {
  auto sodier = std::make_shared<NormalSodier>();
  registerTouchableObject(sodier);
  sodier->setPosition(sf::Vector2f(100, 100));
  sodier->setSpeed(1.f);
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
    if (event.key.code == sf::Keyboard::G) {
      generateSodier();
    } else if (event.key.code == sf::Keyboard::W) {
      soldiers_[0]->setDirection(Direction::Up);
    } else if (event.key.code == sf::Keyboard::S) {
      soldiers_[0]->setDirection(Direction::Down);
    } else if (event.key.code == sf::Keyboard::A) {
      soldiers_[0]->setDirection(Direction::Left);
    } else if (event.key.code == sf::Keyboard::D) {
      soldiers_[0]->setDirection(Direction::Right);
    } else if (event.key.code == sf::Keyboard::Space) {
      soldiers_[0]->setMoving(!soldiers_[0]->isMoving());
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

  // erase dead sodiers
  soldiers_.erase(std::remove_if(soldiers_.begin(), soldiers_.end(),
                                 [](const std::shared_ptr<Sodier>& sodier) {
                                   return sodier->getHealth() <= 0.f;
                                 }),
                  soldiers_.end());
  enemies_.erase(std::remove_if(enemies_.begin(), enemies_.end(),
                                [](const std::shared_ptr<Sodier>& sodier) {
                                  return sodier->getHealth() <= 0.f;
                                }),
                 enemies_.end());
}

void MainScene::generateSodier() {
  auto new_sodier = getDefaultSodier();
  new_sodier->bindHover([=]() { new_sodier->setColor({255, 255, 255, 192}); });
  new_sodier->bindLeave([=]() { new_sodier->setColor({255, 255, 255, 255}); });
  new_sodier->bindClick([=]() { new_sodier->setColor({255, 0, 200, 255}); });
  soldiers_.push_back(std::move(new_sodier));
}

void MainScene::initUi() {
  initMap();
  initBuildings();
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
