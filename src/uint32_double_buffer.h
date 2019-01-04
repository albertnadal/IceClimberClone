#ifndef _UINT32_DOUBLE_BUFFER_H
#define _UINT32_DOUBLE_BUFFER_H

#include <mutex>
#include <types.h>

class uint32DoubleBuffer {

public:
  uint32 *producer_buffer = nullptr;
  uint32 *consumer_buffer = nullptr;
  std::mutex consumer_mutex;
  bool is_consuming_buffer = false;

  uint32DoubleBuffer(uint32);
  void swapBuffers();
  ~uint32DoubleBuffer();
};


#endif
