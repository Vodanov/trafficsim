#include "board.hpp"
#include <raylib.h>
#include <vector>
void board_t::draw_cells() {
  for (auto &row : boardBG)
    for (auto &cell : row)
      cell->draw();
}
void board_t::create_entity(float x, float y, Color col, Vector2 dest) {
  entity_t new_ent(x, y, col);
  std::vector<Vector2> paths = find_path({x, y}, dest);
  new_ent._path = std::move(paths);
  entities.push_back(new_ent);
}
void board_t::draw_entities() {
  for (auto &entity : entities) {
    entity.draw();
  }
}
void board_t::draw_board() {
  draw_cells();
  draw_entities();
}
std::unique_ptr<cell_t> &board_t::at(u32 idx, u32 idx2) {
  return boardBG.at(idx).at(idx2);
}
std::vector<Vector2> board_t::find_path(Vector2 start, Vector2 end) {}