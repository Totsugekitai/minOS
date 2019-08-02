#include <stdint.h>
#include <util/util.h>

uint64_t pow(uint64_t num, uint64_t pow)
{
    uint64_t i, ans = 1;
    for (i = 0; i < pow; i++) {
        ans *= num;
    }
    return ans;
}

struct ring_buffer gen_ring_buf(void)
{
    struct ring_buffer buf;
    for (int i = 0; i < BUF_SIZE; i++) {
        buf.queue[i] = 0;
    }
    buf.head = 0;
    buf.tail = 0;
    return buf;
}

uint8_t buf_isempty(struct ring_buffer *buf)
{
    return buf->head == buf->tail;
}

uint8_t buf_isfull(struct ring_buffer *buf)
{
    return (buf->tail + 1) % BUF_SIZE == buf->head;
}

uint8_t enqueue(struct ring_buffer *buf, uint64_t data)
{
    if (buf_isfull(buf)) {
        return 0;
    }
    buf->queue[buf->tail] = data;
    buf->tail = (buf->tail + 1) % BUF_SIZE;
    return 1;
}

uint8_t dequeue(struct ring_buffer *buf, uint64_t *dst)
{
    if (buf_isempty(buf)) {
        return 0;
    }
    *dst = buf->queue[buf->head];
    buf->head = (buf->head + 1) % BUF_SIZE;
    return 1;
}

void flush_buf(struct ring_buffer *buf)
{
    for (int i = 0; i < BUF_SIZE; i++) {
        buf->queue[i] = 0;
    }
    buf->head = 0;
    buf->tail = 0;
}
