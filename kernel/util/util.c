#include <stdint.h>
#include <util/util.h>

/* memory関連 */
void *memset(void *s, int c, int n)
{
    uint8_t *tmp = s;
    uint8_t ch = c;
    for (int i = 0; i < n; i++) {
        tmp[i] = ch;
    }
    return s;
}

void *memcpy(void *buf1, const void *buf2, int n)
{
    char *b1 = buf1;
    const char *b2 = buf2;
    for (int i = 0; i < n; i++) {
        b1[i] = b2[i];
    }
    return buf1;
}

/* 数学関数 */
uint64_t pow(uint64_t num, uint64_t pow)
{
    uint64_t ans = 1;
    for (uint64_t i = 0; i < pow; i++) {
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

void flush_array_char(char *chararr)
{
    uint8_t i = 0;
    while (chararr[i] != 0x00) {
        chararr[i] = 0x00;
        i++;
    }
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

int8_t strncmp(char *str1, char *str2, int num)
{
    for (int i = 0; i < num; i++) {
        if (str1[i] > str2[i]) {
            return 1;
        } else if (str1[i] < str2[i]) {
            return -1;
        } else {
            continue;
        }
    }
    return 0;
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

void u64_to_hexstr(char dst[19], uint64_t num)
{
    uint64_t i;
    uint64_t numarr[16];
    for (i = 0; i < 16; i++) {
        numarr[i] = (num & (pow(2, 0 + i * 4) + pow(2, 1 + i * 4) + pow(2, 2 + i * 4) + pow(2, 3 + i * 4))) >> i * 4;
    }
    dst[0] = '0';
    dst[1] = 'x';
    dst[18] = '\0';
    for (i = 0; i < 16; i++)
    {
        if (numarr[i] == 0x0)
        {
            dst[17 - i] = '0';
        }
        if (numarr[i] == 0x1)
        {
            dst[17 - i] = '1';
        }
        if (numarr[i] == 0x2)
        {
            dst[17 - i] = '2';
        }
        if (numarr[i] == 0x3)
        {
            dst[17 - i] = '3';
        }
        if (numarr[i] == 0x4)
        {
            dst[17 - i] = '4';
        }
        if (numarr[i] == 0x5)
        {
            dst[17 - i] = '5';
        }
        if (numarr[i] == 0x6)
        {
            dst[17 - i] = '6';
        }
        if (numarr[i] == 0x7)
        {
            dst[17 - i] = '7';
        }
        if (numarr[i] == 0x8)
        {
            dst[17 - i] = '8';
        }
        if (numarr[i] == 0x9)
        {
            dst[17 - i] = '9';
        }
        if (numarr[i] == 0xa)
        {
            dst[17 - i] = 'a';
        }
        if (numarr[i] == 0xb)
        {
            dst[17 - i] = 'b';
        }
        if (numarr[i] == 0xc)
        {
            dst[17 - i] = 'c';
        }
        if (numarr[i] == 0xd)
        {
            dst[17 - i] = 'd';
        }
        if (numarr[i] == 0xe)
        {
            dst[17 - i] = 'e';
        }
        if (numarr[i] == 0xf)
        {
            dst[17 - i] = 'f';
        }
    }
}

uint64_t demchar_to_u64(char demchar)
{
    if (demchar == '0') {
        return 0;
    }
    if (demchar == '1') {
        return 1;
    }
    if (demchar == '2') {
        return 2;
    }
    if (demchar == '3') {
        return 3;
    }
    if (demchar == '4') {
        return 4;
    }
    if (demchar == '5') {
        return 5;
    }
    if (demchar == '6') {
        return 6;
    }
    if (demchar == '7') {
        return 7;
    }
    if (demchar == '8') {
        return 8;
    }
    if (demchar == '9') {
        return 9;
    }
    return 0;
}

uint64_t demstr_to_u64(char *demstr)
{
    uint8_t digit = 0;
    uint64_t num = 0;
    // 桁数解析
    while (demstr[digit] != 0x00) {
        digit++;
    }
    // 各桁を変換して配列にしまう
    uint64_t demarr[digit];
    for (int i = 0; i < digit; i++) {
        demarr[i] = demchar_to_u64(demstr[digit - 1 - i]);
    }
    // uint64_tに変換
    for (int i = 0; i < digit; i++) {
        num += demarr[i] * pow(10, i);
    }
    return num;
}

void null_to_space(char *str, int len)
{
    for (int i = 0; i < len; i++) {
        if (str[i] == 0x00) {
            str[i] = 0x20;
        }
    }
}

