#pragma once
#include <stdint.h>

#define BUF_SIZE    64
struct ring_buffer {
    uint64_t queue[BUF_SIZE];
    uint8_t head;
    uint8_t tail;
};

uint64_t pow(uint64_t num, uint64_t pow);
struct ring_buffer gen_ring_buf(void);
uint8_t buf_isempty(struct ring_buffer *buf);
uint8_t buf_isfull(struct ring_buffer *buf);
uint8_t enqueue(struct ring_buffer *buf, uint64_t data);
uint8_t dequeue(struct ring_buffer *buf, uint64_t *dst);
void flush_buf(struct ring_buffer *buf);
