#pragma once
#include "gameSettings.hpp"
#include "entity.hpp"
#include <raylib.h>
//     void DrawRectanglePro(Rectangle rec, Vector2 origin, float rotation, Color color);                 // Draw a color-filled rectangle with pro parameters
class entity_t {
public:
    u32 _x{0}, _y{0};
    Color _col{0,0,0,0};
    float _width{cellSizeX}, _height{cellSizeY};
    u8 dir{1}, _speed{0}, _madness{0}, _type{0};
    entity_t(){
        get_id();
    }
    entity_t(u32 x, u32 y) : _x(x), _y(y) {
        get_id();
    }
    entity_t(u32 x, u32 y, Color col) : _x(x), _y(y), _col(col){
        get_id();
    }
    void draw(){
        DrawRectangle(_x, _y, cellSizeX, cellSizeY, _col);
    }
private:
    static u32 counter;
    u32 id;
    void get_id(){
        id = counter;
        counter++;
    }
};
class cell_t {
public:
    void set(u8 i){
        _c = i;
        if (i == 0){
            _col = {0, 0,0,255};
        }else if (i == 1){
            _col = {0, 128,0,255};
        }else if (i == 2){
            _col = {0, 0,128,255};
        }else if (i == 3){
            _col = {255, 255,255,255};
        }else {
            _col = {128, 0,0,255};
        }
    }
    void set(Color col){
        _col = col;
    }
    void set(){
        if (_c == 0){
            _col = {0, 0,0,255};
        }else if (_c == 1){
            _col = {0, 128,0,255};
        }else if (_c == 2){
            _col = {0, 0,128,255};
        }else if (_c == 3){
            _col = {255, 255,255,255};
        }else {
            _col = {128, 0,0,255};
        }
        _c++;
    }
    void move(u8 dir) {
        u32 i = _x / cellSizeX;
        u32 j = _y / cellSizeY;
    }
    char info(){
      return _c;
    }
    void draw(){
        set(_c);
        DrawRectangle(_x, _y, cellSizeX, cellSizeY, _col);
    }
    cell_t(u32 posX, u32 posY) : _x(posX), _y(posY){}
    u16 _x, _y;
    Color _col{0,0,0,255};
    u8 _c{0};
};
