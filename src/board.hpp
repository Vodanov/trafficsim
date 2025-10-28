#pragma once
#include <fstream>
#include "entity.hpp"
#include "gameSettings.hpp"
#include "includes.hpp"
#include <memory>
#include <string>
#include "entity.hpp"
#include "entity.cpp"
class board_t{
public:
    auto& at(u32 idx){
        return boardBG.at(idx);
    }
    void draw_cells(){
        for (auto& row : boardBG)
            for(auto& cell : row)
                cell->draw();
    }
    void draw_entities(){
        for(auto& row : entities)
            for(auto& entity : row)
                entity->draw();         
                //DrawRectanglePro({entity->_x, entity->_y, entity->_width, entity->_height},{cellSizeX/2.0f, cellSizeY/2.0f}, entity->rotation, entity->_color);
    }
    board_t(){
        for(uint32_t i = 0; i < screenHeight; i += cellSizeY){
            std::vector<std::unique_ptr<cell_t>> row;
            for(uint32_t j = 0; j < screenWidth; j += cellSizeX){
                row.push_back(std::make_unique<cell_t>(j,i));
            }
            boardBG.push_back(std::move(row));
        }
    }
    board_t(const std::string& File){
      std::ifstream file(File);
      std::string line;
      u32 i = 0;
      while(std::getline(file, line)){
        std::vector<std::unique_ptr<cell_t>> row;
        u32 j = 0;
        for(auto& c : line){
          row.push_back(std::make_unique<cell_t>(j, i));
          row[j]->set(c);
          j++;
        }
        boardBG.push_back(std::move(row));
        i++;
      }
    }
    void draw_board(){
        draw_cells();
        draw_entities();
    }
    ~board_t(){
      std::ofstream file("board.tmp");
      for(auto& row : boardBG){
        for(auto& cell : row){
          file << cell->info();
        }
        file << std::endl;
      }
      file.close();
    }
private:
    std::vector<std::vector<std::unique_ptr<cell_t>>> boardBG; 
    std::vector<std::vector<std::unique_ptr<entity_t>>> entities;
};
