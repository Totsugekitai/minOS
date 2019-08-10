#include <stdint.h>
#include <util/util.h>

/* 数学関数 */
uint64_t pow(uint64_t num, uint64_t pow)
{
    uint64_t i, ans = 1;
    for (i = 0; i < pow; i++) {
        ans *= num;
    }
    return ans;
}

/* データ構造 */
struct ring_buf_u64 gen_buf_u64(void)
{
    struct ring_buf_u64 buf;
    for (int i = 0; i < BUF_SIZE; i++) {
        buf.queue[i] = 0;
    }
    buf.head = 0;
    buf.tail = 0;
    return buf;
}

uint8_t buf_u64_isempty(struct ring_buf_u64 *buf)
{
    return buf->head == buf->tail;
}

uint8_t buf_u64_isfull(struct ring_buf_u64 *buf)
{
    return (buf->tail + 1) % BUF_SIZE == buf->head;
}

uint8_t enqueue_u64(struct ring_buf_u64 *buf, uint64_t data)
{
    if (buf_u64_isfull(buf)) {
        return 0;
    }
    buf->queue[buf->tail] = data;
    buf->tail = (buf->tail + 1) % BUF_SIZE;
    return 1;
}

uint8_t dequeue_u64(struct ring_buf_u64 *buf, uint64_t *dst)
{
    if (buf_u64_isempty(buf)) {
        return 0;
    }
    *dst = buf->queue[buf->head];
    buf->head = (buf->head + 1) % BUF_SIZE;
    return 1;
}

void flush_buf_u64(struct ring_buf_u64 *buf)
{
    for (int i = 0; i < BUF_SIZE; i++) {
        buf->queue[i] = 0;
    }
    buf->head = 0;
    buf->tail = 0;
}

struct ring_buf_char gen_buf_char(void)
{
    struct ring_buf_char buf;
    for (int i = 0; i < BUF_SIZE; i++) {
        buf.queue[i] = 0;
    }
    buf.head = 0;
    buf.tail = 0;
    return buf;
}

uint8_t buf_char_isempty(struct ring_buf_char *buf)
{
    return buf->head == buf->tail;
}

uint8_t buf_char_isfull(struct ring_buf_char *buf)
{
    return (buf->tail + 1) % BUF_SIZE == buf->head;
}

uint8_t enqueue_char(struct ring_buf_char *buf, char data)
{
    if (buf_char_isfull(buf)) {
        return 0;
    }
    buf->queue[buf->tail] = data;
    buf->tail = (buf->tail + 1) % BUF_SIZE;
    return 1;
}

uint8_t dequeue_char(struct ring_buf_char *buf, char *dst)
{
    if (buf_char_isempty(buf)) {
        return 0;
    }
    *dst = buf->queue[buf->head];
    buf->head = (buf->head + 1) % BUF_SIZE;
    return 1;
}

void flush_buf_char(struct ring_buf_char *buf)
{
    for (int i = 0; i < BUF_SIZE; i++) {
        buf->queue[i] = 0;
    }
    buf->head = 0;
    buf->tail = 0;
}

/* 文字列 */
uint8_t comptext(char *text, char *pat)
{
    // textとpatの長さを比較して等しかったら続ける
    int i = 0, j = 0;
    while (text[i] != 0x00) {
        i++;
    }
    while (pat[j] != 0x00) {
        j++;
    }
    if (i == j) {
        return 0;
    }

    // 一文字ずつ等しいか確認する
    for (i = 0; i < j; i++) {
        if (text[i] != pat[i]) {
            return 0;
        }
    }

    // 等しかったら1を返す
    return 1;
}

uint8_t strncmp(char *str1, char *str2, int num)
{
    for (int i = 0; i < num; i++) {
        if (str1[i] != str2[i]) {
            return 0;
        }
    }
    return 1;
}

void sprintf(char *src, char *dst)
{
    uint8_t i = 0;
    while (src[i] != 0x00) {
        dst[i] = src[i];
        i++;
    }
    dst[i] = src[i];
}
