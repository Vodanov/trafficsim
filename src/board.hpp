#include "cell.hpp"
#include "definitions.hpp"
#include "entity.hpp"
#include <bitset>
#include <raylib.h>
#include <stack>
#include <vector>
#include "visible_area.hpp"
class board_t {
public:
  cell_t &at(i32 idx, i32 idx2);
  void create_entity(float x, float y, Color col);
  void draw_board(u8 &pause, VisibleArea area);
  void size();
  void create_desitation(float x, float y);
  board_t();
  ~board_t();
private:
  void bfs(std::bitset<tableWidth * tableHeight> &table, Vector2 &end, Vector2 &start,
           std::stack<Vector2> &path);
  void draw_cells(u8 &pause, VisibleArea area);
  void draw_entities(u8 &pause);
  bool valid_road(i32 x, i32 y);
  std::vector<std::vector<cell_t>> boardBG;
  bool cant_move(i32 x, i32 y, Vector2 prev);
  std::vector<entity_t> entities;
  std::stack<Vector2> find_path(Vector2 start, Vector2 end);
  std::vector<Vector2> _destinations {};
};