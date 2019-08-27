#include "float_double_buffer.h"

FloatDoubleBuffer::FloatDoubleBuffer(uint32_t _length) {
        length = _length;
        producer_buffer = (float *)calloc(_length, sizeof(float));
        consumer_buffer = (float *)calloc(_length, sizeof(float));
}

uint32_t FloatDoubleBuffer::size()
{
        return (length * sizeof(float));
}

void FloatDoubleBuffer::cleanDataFromPosition(uint32_t position)
{
  std::memset(producer_buffer+position, 0, length-position);
}

void FloatDoubleBuffer::lock()
{
        consumer_mutex.lock();
        is_consuming_buffer = true;
}

void FloatDoubleBuffer::unlock()
{
        is_consuming_buffer = false;
        consumer_mutex.unlock();
}

void FloatDoubleBuffer::swapBuffers()
{
        if(!is_consuming_buffer) {
                consumer_mutex.lock();
                float *tmp = producer_buffer;
                producer_buffer = consumer_buffer;
                consumer_buffer = tmp;
                consumer_mutex.unlock();
        }
}

FloatDoubleBuffer::~FloatDoubleBuffer() {

        if(producer_buffer != nullptr) {
                delete[] producer_buffer;
        }

        if(consumer_buffer != nullptr) {
                delete[] consumer_buffer;
        }
}
