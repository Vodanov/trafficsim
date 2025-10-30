#pragma once
#include "cell.cpp"
#include "entity.cpp"
#include "entity.hpp"
#include "gameSettings.hpp"
#include <fstream>
#include <memory>
#include <string>
class board_t {
public:
  inline auto &at(u32 idx) { return boardBG.at(idx); }
  void draw_cells() {
    for (auto &row : boardBG)
      for (auto &cell : row)
        cell->draw();
  }
  void draw_entities() {
    for (auto &entity : entities) {
      entity->draw();
    }
  }
  void create_entity(u32 x, u32 y, Color col) {
    entities.push_back(std::make_unique<entity_t>(x, y, col));
  }

  board_t() {
    std::ifstream file("board.brd");
    if (!file.is_open()) {
      for (uint32_t i = 0; i < screenHeight; i += cellSizeY) {
        std::vector<std::unique_ptr<cell_t>> row;
        for (uint32_t j = 0; j < screenWidth; j += cellSizeX) {
          row.push_back(std::make_unique<cell_t>(j, i));
        }
        boardBG.push_back(std::move(row));
      }
      return;
    }
    std::string line;
    u32 i = 0;
    while (std::getline(file, line)) {
      std::vector<std::unique_ptr<cell_t>> row;
      u32 idx = 0;
      u32 j = 0;
      for (auto &c : line) {
        row.push_back(std::make_unique<cell_t>(j, i));
        row[idx]->set(c);
        j += cellSizeX;
        idx++;
      }
      boardBG.push_back(std::move(row));
      i += cellSizeY;
    }
  }
  void draw_board() {
    draw_cells();
    draw_entities();
  }
  ~board_t() {
    std::ofstream file("board.brd");
    for (auto &row : boardBG) {
      for (auto &cell : row) {
        file << cell->info();
      }
      file << '\n';
    }
    file.close();
  }

private:
  std::vector<std::vector<std::unique_ptr<cell_t>>> boardBG;
  std::vector<std::unique_ptr<entity_t>> entities;
};