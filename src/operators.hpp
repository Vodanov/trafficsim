#pragma once
#include <raylib.h>
inline bool operator==(const Vector2 &a, const Vector2 &b) {
  return a.x == b.x && a.y == b.y;
}
inline bool operator==(const Camera2D &a, const Camera2D &b) {
  return (a.offset == b.offset && a.rotation == b.rotation &&
          a.target == b.target && a.zoom == b.zoom);
}
inline bool operator!=(const Vector2 &a, const Vector2 &b) {
  return a.x != b.x || a.y != b.y;
}
inline bool operator<(const Vector2 &a, const Vector2 &b) {
  return (a.x < b.x || (a.x == b.x && a.y < b.y));
}
inline Vector2 operator-(const Vector2 &a, const Vector2 &b) {
  return {a.x - b.x, a.y - b.y};
}
inline Vector2 operator+(const Vector2 &a, const Vector2 &b) {
  return {a.x + b.x, a.y + b.y};
}