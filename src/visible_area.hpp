#pragma once
#include <algorithm>
#include <raylib.h>
#include "definitions.hpp"
struct VisibleArea {
    int startX, endX;
    int startY, endY;
    VisibleArea(const Camera2D& camera){
      Rectangle viewport = {
        -camera.offset.x / camera.zoom,
        -camera.offset.y / camera.zoom,
        (float)GetScreenWidth() / camera.zoom,
        (float)GetScreenHeight() / camera.zoom
    };
    startX = std::max(0, (i32)(viewport.x / cellSizeX));
    startY = std::max(0, (i32)(viewport.y / cellSizeY));
    endX = std::min(boardWidth / cellSizeX, (u32)((viewport.x + viewport.width) / cellSizeX) + 1);
    endY = std::min(boardHeight / cellSizeY, (u32)((viewport.y + viewport.height) / cellSizeY) + 1);
    }
    VisibleArea() = default;
};