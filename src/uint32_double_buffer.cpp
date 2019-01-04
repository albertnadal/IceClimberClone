#include "uint32_double_buffer.h"

uint32DoubleBuffer::uint32DoubleBuffer(uint32 _length) {
        producer_buffer = (uint32 *)calloc(_length, sizeof(uint32));
        consumer_buffer = (uint32 *)calloc(_length, sizeof(uint32));
}

void uint32DoubleBuffer::swapBuffers()
{
        if(!is_consuming_buffer) {
                consumer_mutex.lock();
                uint32 *tmp = producer_buffer;
                producer_buffer = consumer_buffer;
                consumer_buffer = tmp;
                consumer_mutex.unlock();
        }
}

uint32DoubleBuffer::~uint32DoubleBuffer() {

        if(producer_buffer != nullptr) {
                delete[] producer_buffer;
        }

        if(consumer_buffer != nullptr) {
                delete[] consumer_buffer;
        }
}
