#include <stdint.h>
#include <device/device.h>
#include <command/command.h>
#include <types/boottypes.h>
#include <graphics/graphics.h>
#include <util/util.h>

#define MAX_ARGS    8
#define ARG_LENGTH  8
#define OUTPUT_LENGTH   64

extern struct pix_format white;
extern struct pix_format black;
extern struct pix_format green;
extern struct video_info *vinfo_global;

uint8_t keycode;
uint32_t text_x = 0, text_y = 0;

void readline_serial(struct ring_buf_char *text_buf)
{
    keycode = 0x00; // 初期化
    putstr(text_x, text_y, green, black, vinfo_global, "totsugeki@minOS $ ");
    text_x += 8 * 18;
    while (keycode != 0x0d) { // 改行が来たらやめる
        keycode = read_serial();
        if (keycode == 0x0d) {
            break;
        }
        if (!buf_char_isfull(text_buf)) {
            enqueue_char(text_buf, keycode);
            // 文字描画部
            putchar(text_x, text_y, white, black, vinfo_global, keycode);
            text_x += 8;
        }
    }
    // 改行処理
    text_x = 0;
    text_y += 16;
}

void parse_line(struct ring_buf_char *buf, char *args_array, char *args_top[10])
{
    // バッファに入っている文字をargs_arrayに移動
    uint8_t char_count = 0;
    while (!buf_char_isempty(buf)) {
        dequeue_char(buf, &args_array[char_count]);
        char_count++;
    }
    // args_arrayに入った文字を解析
    char_count++;
    uint8_t i, j = 0;
    args_top[j] = args_array;
    j++;
    for (i = 0; i < char_count; i++) {
        if (args_array[i] == 0x20) { // スペースだったら
            args_top[j] = &args_array[i + 1]; // args_topに次の文字のアドレスを登録
            args_array[i] = 0x00; // スペースをnull文字に置換
            j++;
        }
    }
    // 最後の改行文字を置き換え
    args_array[i] = 0x00;
}

void do_command(char *args_array, char *args_top[MAX_ARGS], char *output)
{
    // コマンドはargs_arrayの先頭
    char *command = args_array;
    char null_char = 0x00;
    // コマンド引数の数を解析
    int number_of_args = 0;
    for (int i = 1; i < MAX_ARGS; i++) {
        if (!args_top[i] != 0) {
            number_of_args++;
        }
    }
    // コマンドによって実行コマンドを振り分ける
    if (strncmp(command, "echo", 5)) {
        sprintf("echo~~~", output);
    } else if (strncmp(command, &null_char, 1)) {
        sprintf("no input", output);
    } else {
        sprintf("bad command", output);
    }
}

void writelines(char *output)
{
    putstr(text_x, text_y, white, black, vinfo_global, output);
    text_x = 0;
    text_y += 16;
}

void console(void)
{
    struct ring_buf_char text_buf = gen_buf_char();
    char args_array[MAX_ARGS * ARG_LENGTH] = {0},
        *args_top[MAX_ARGS] = {0}, output[OUTPUT_LENGTH] = {0};

    readline_serial(&text_buf);
    parse_line(&text_buf, args_array, args_top);
    do_command(args_array, args_top, output);
    writelines(output);

    flush_buf_char(&text_buf);
}

