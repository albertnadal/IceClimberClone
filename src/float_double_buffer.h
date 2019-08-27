#ifndef _FLOAT_DOUBLE_BUFFER_H
#define _FLOAT_DOUBLE_BUFFER_H

#include <mutex>
#include <defines.h>

class FloatDoubleBuffer {

public:
  uint32_t length;
  float *producer_buffer = nullptr;
  float *consumer_buffer = nullptr;
  std::mutex consumer_mutex;
  bool is_consuming_buffer = false;

  FloatDoubleBuffer(uint32_t);
  uint32_t size();
  void cleanDataFromPosition(uint32_t);
  void lock();
  void unlock();
  void swapBuffers();
  ~FloatDoubleBuffer();
};


#endif
