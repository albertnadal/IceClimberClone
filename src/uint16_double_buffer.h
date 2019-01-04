#ifndef _UINT16_DOUBLE_BUFFER_H
#define _UINT16_DOUBLE_BUFFER_H

#include <mutex>
#include <types.h>

class UInt16DoubleBuffer {

public:
  uint32 length;
  uint16 *producer_buffer = nullptr;
  uint16 *consumer_buffer = nullptr;
  std::mutex consumer_mutex;
  bool is_consuming_buffer = false;

  UInt16DoubleBuffer(uint32);
  uint32 size();
  void swapBuffers();
  void lock();
  void unlock();
  ~UInt16DoubleBuffer();
};


#endif
