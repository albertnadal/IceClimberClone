#ifndef _UINT16_DOUBLE_BUFFER_H
#define _UINT16_DOUBLE_BUFFER_H

#include <mutex>
#include <defines.h>

class UInt16DoubleBuffer {

public:
  uint32_t length;
  uint16_t *producer_buffer = nullptr;
  uint16_t *consumer_buffer = nullptr;
  std::mutex consumer_mutex;
  bool is_consuming_buffer = false;

  UInt16DoubleBuffer(uint32_t);
  uint32_t size();
  void cleanDataFromPosition(uint32_t);
  void swapBuffers();
  void lock();
  void unlock();
  ~UInt16DoubleBuffer();
};


#endif
