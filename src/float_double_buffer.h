#ifndef _FLOAT_DOUBLE_BUFFER_H
#define _FLOAT_DOUBLE_BUFFER_H

#include <mutex>
#include <defines.h>

class FloatDoubleBuffer {

public:
  uint32 length;
  float *producer_buffer = nullptr;
  float *consumer_buffer = nullptr;
  std::mutex consumer_mutex;
  bool is_consuming_buffer = false;

  FloatDoubleBuffer(uint32);
  uint32 size();
  void cleanDataFromPosition(uint32);
  void lock();
  void unlock();
  void swapBuffers();
  ~FloatDoubleBuffer();
};


#endif
