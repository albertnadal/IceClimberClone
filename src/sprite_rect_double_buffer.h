#ifndef _SPRITE_RECT_DOUBLE_BUFFER_H
#define _SPRITE_RECT_DOUBLE_BUFFER_H

#include <mutex>
#include <defines.h>
#include <raylib/raylib.h>

struct SpriteRect {
    Rectangle source;
    Vector2 position;

    SpriteRect() : source({0,0,0,0}), position({0,0}) {}
    SpriteRect(Rectangle src, Vector2 pos) : source(src), position(pos) {}
};

class SpriteRectDoubleBuffer {

public:
  uint32_t max_length;
  uint32_t producer_buffer_length;
  uint32_t consumer_buffer_length;
  SpriteRect* producer_buffer = nullptr;
  SpriteRect* consumer_buffer = nullptr;
  //uint16_t *producer_buffer = nullptr;
  //uint16_t *consumer_buffer = nullptr;
  std::mutex consumer_mutex;
  bool is_consuming_buffer = false;

  SpriteRectDoubleBuffer(uint32_t);
  uint32_t buffer_size();
  void swapBuffers();
  void lock();
  void unlock();
  ~SpriteRectDoubleBuffer();
};


#endif