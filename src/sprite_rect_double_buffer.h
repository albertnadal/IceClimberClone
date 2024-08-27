#ifndef _SPRITE_RECT_DOUBLE_BUFFER_H
#define _SPRITE_RECT_DOUBLE_BUFFER_H

#include <mutex>
#include <defines.h>
#include <entity.h>
#include <raylib/raylib.h>

struct SpriteRect {
    Rectangle source;
    Vector2 position;
    Boundaries boundaries; // Used only for debug purposes.
    Boundaries attackBoundaries; // Used only for debug purposes.
    Color tint;  // Used only for debug purposes.

    SpriteRect() : source({0,0,0,0}), position({0,0}), boundaries({0,0,0,0}), attackBoundaries({0,0,0,0}), tint(WHITE) {}
    SpriteRect(Rectangle src, Vector2 pos, Boundaries boundaries, Boundaries attackBoundaries, Color tint) : source(src), position(pos), boundaries(boundaries), attackBoundaries(attackBoundaries), tint(tint) {}
};

class SpriteRectDoubleBuffer {

public:
  uint32_t max_length;
  uint32_t producer_buffer_length;
  uint32_t consumer_buffer_length;
  SpriteRect* producer_buffer = nullptr;
  SpriteRect* consumer_buffer = nullptr;
  std::mutex consumer_mutex;
  bool is_consuming_buffer = false;

  SpriteRectDoubleBuffer(uint32_t);
  uint32_t bufferSize() const;
  void swapBuffers();
  void lock();
  void unlock();
  ~SpriteRectDoubleBuffer();
};


#endif
