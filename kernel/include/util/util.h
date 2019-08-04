#pragma once
#include <stdint.h>

#define BUF_SIZE    64
struct ring_buf_u64 {
    uint64_t queue[BUF_SIZE];
    uint8_t head;
    uint8_t tail;
};
struct ring_buf_char {
    char queue[BUF_SIZE];
    uint8_t head;
    uint8_t tail;
};

uint64_t pow(uint64_t num, uint64_t pow);


struct ring_buf_u64 gen_buf_u64(void);
uint8_t buf_u64_isempty(struct ring_buf_u64 *buf);
uint8_t buf_u64_isfull(struct ring_buf_u64 *buf);
uint8_t enqueue_u64(struct ring_buf_u64 *buf, uint64_t data);
uint8_t dequeue_u64(struct ring_buf_u64 *buf, uint64_t *dst);
void flush_buf_u64(struct ring_buf_u64 *buf);

struct ring_buf_char gen_buf_char(void);
uint8_t buf_char_isempty(struct ring_buf_char *buf);
uint8_t buf_char_isfull(struct ring_buf_char *buf);
uint8_t enqueue_char(struct ring_buf_char *buf, char data);
uint8_t dequeue_char(struct ring_buf_char *buf, char *dst);
void flush_buf_char(struct ring_buf_char *buf);

uint8_t comptext(char *text, char *pat);

