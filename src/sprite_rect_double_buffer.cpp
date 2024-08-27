#include <sprite_rect_double_buffer.h>

SpriteRectDoubleBuffer::SpriteRectDoubleBuffer(uint32_t _max_length) {
        max_length = _max_length;
        producer_buffer_length = 0;
        consumer_buffer_length = 0;
        producer_buffer = new SpriteRect[max_length];
        consumer_buffer = new SpriteRect[max_length];
}

uint32_t SpriteRectDoubleBuffer::bufferSize() const
{
        return (max_length * sizeof(SpriteRect));
}

void SpriteRectDoubleBuffer::lock()
{
        consumer_mutex.lock();
        is_consuming_buffer = true;
}

void SpriteRectDoubleBuffer::unlock()
{
        is_consuming_buffer = false;
        consumer_mutex.unlock();
}

void SpriteRectDoubleBuffer::swapBuffers()
{
        if(!is_consuming_buffer) {
                consumer_mutex.lock();
                SpriteRect *tmp_buffer = producer_buffer;
                producer_buffer = consumer_buffer;
                consumer_buffer = tmp_buffer;
                consumer_buffer_length = producer_buffer_length;
                consumer_mutex.unlock();
        }
}

SpriteRectDoubleBuffer::~SpriteRectDoubleBuffer() {

        if(producer_buffer != nullptr) {
                delete[] producer_buffer;
        }

        if(consumer_buffer != nullptr) {
                delete[] consumer_buffer;
        }
}
