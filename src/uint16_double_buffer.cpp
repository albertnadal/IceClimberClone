#include "uint16_double_buffer.h"

UInt16DoubleBuffer::UInt16DoubleBuffer(uint32 _length) {
        length = _length;
        producer_buffer = (uint16 *)calloc(_length, sizeof(uint16));
        consumer_buffer = (uint16 *)calloc(_length, sizeof(uint16));
}

uint32 UInt16DoubleBuffer::size()
{
        return (length * sizeof(uint16));
}

void UInt16DoubleBuffer::cleanDataFromPosition(uint32 position)
{
  std::memset(producer_buffer+position, 0, length-position);
}

void UInt16DoubleBuffer::lock()
{
        consumer_mutex.lock();
        is_consuming_buffer = true;
}

void UInt16DoubleBuffer::unlock()
{
        is_consuming_buffer = false;
        consumer_mutex.unlock();
}

void UInt16DoubleBuffer::swapBuffers()
{
        if(!is_consuming_buffer) {
                consumer_mutex.lock();
                uint16 *tmp = producer_buffer;
                producer_buffer = consumer_buffer;
                consumer_buffer = tmp;
                consumer_mutex.unlock();
        }
}

UInt16DoubleBuffer::~UInt16DoubleBuffer() {

        if(producer_buffer != nullptr) {
                delete[] producer_buffer;
        }

        if(consumer_buffer != nullptr) {
                delete[] consumer_buffer;
        }
}
