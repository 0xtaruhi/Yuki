#include "MainScene.hpp"

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
}

void MainScene::draw() const {
  window_.draw(map_);

  for (auto& soldier : soldiers_) {
    window_.draw(*soldier);
  }
  for (auto& enemy : enemies_) {
    window_.draw(*enemy);
  }
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
  new_sodier->bindHover([=]() { new_sodier->setHealth(2.f); });
  soldiers_.push_back(std::move(new_sodier));
}

void MainScene::initUi() {
  // map
  map_.setSize({30, 20});
  map_.setTileSize({32, 32});
  
  // road
  for (int i = 7; i != 20; ++i) {
    map_.setTile(3, i, TileInfo(TileCategory::Road, 4));
    // map_.setTile(3, i, TileInfo(TileCategory::Road, 5));
    map_.setTile(4, i, TileInfo(TileCategory::Road, 6));

    map_.setTile(25, i, TileInfo(TileCategory::Road, 4));
    // map_.setTile(26, i, TileInfo(TileCategory::Road, 5));
    map_.setTile(26, i, TileInfo(TileCategory::Road, 6));
  }

  // trees
  map_.setTile(13,16, TileInfo(TileCategory::Tree, 3));
  map_.setTile(12,15, TileInfo(TileCategory::Tree, 3));

}
