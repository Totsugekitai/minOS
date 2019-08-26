#include <stdint.h>
#include <core/global_variables.h>
#include <device/device.h>
#include <command/command.h>
#include <types/boottypes.h>
#include <graphics/graphics.h>
#include <util/util.h>
#include <app/app.h>

struct task_queue task_q;
volatile int readline_flag = 0;
volatile char readline_buf[READLINE_BUF_LENGTH] = {0};

uint8_t keycode, oldkeycode, shift = 0;
uint32_t text_x = 0, text_y = 0;

void readline_serial(struct ring_buf_char *text_buf)
{
    keycode = 0x00; // 初期化
    putstr(text_x, text_y, green, black, vinfo_global, "totsugeki@minOS $ ");
    text_x += 8 * 18;
    while (keycode != 0x0d) { // 改行が来たらやめる
        if (readline_flag == 0) {
            keycode = read_serial(readline_flag);
            if (keycode == 0x0d) {
                break;
            }
            if (keycode == 0x0a) {
                continue;
            }
            if (!buf_char_isfull(text_buf)) {
                enqueue_char(text_buf, keycode);
                // 文字描画部
                putnum(300, 100, white, black, vinfo_global, keycode);
                putchar(text_x, text_y, white, black, vinfo_global, keycode);
                text_x += 8;
            }
        } else {
            while (!buf_char_isempty(text_buf)) {
                dequeue_char(text_buf, 0x00);
            }
            for (int i = 0; i < READLINE_BUF_LENGTH; i++) {
                if (readline_buf[i] != 0x00) {
                    enqueue_char(text_buf, readline_buf[i]);
                } else {
                    break;
                }
            }
            readline_flag = 0;
            puts_serial("readline_flag changes 0\n");
            break;
        }
    }
    // 改行処理
    text_x = 0;
    text_y += 16;
}

void readline_ps2(struct ring_buf_char *text_buf)
{
    keycode = 0x00; // 初期化
    putstr(text_x, text_y, green, black, vinfo_global, "totsugeki@minOS $ ");
    text_x += 8 * 18;
    while (keycode != 0x0a) { // 改行が来たらやめる
        keycode = map_scan_to_ascii_set1(ps2_received(), &shift);
        if (keycode == 0x0a) {
            break;
        }
        if (keycode == 0x00) {
            continue;
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
    puts_serial(args_array);
    puts_serial("\n");
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
    for (int i = 0; i < MAX_ARGS; i++) {
        if (args_top[i] != 0) {
            number_of_args++;
        }
    }
    // コマンドによって実行コマンドを振り分ける
    if (strncmp(command, "echo", 5) == 0) {
        if (number_of_args != 2) {
            sprintf("echo: bad args", output);
        } else {
            puts_serial("echo\n");
            echo(args_top[1], output);
        }
    } else if (strncmp(command, "uptime", 7) == 0) {
        if (number_of_args != 1) {
            sprintf("uptime: bad args", output);
        } else {
            puts_serial("uptime\n");
            uptime(output);
        }
    } else if (strncmp(command, "sleep", 6) == 0) {
        if (number_of_args != 2) {
            sprintf("sleep: bad args", output);
        } else {
            puts_serial("sleep\n");
            sleep(demstr_to_u64(args_top[1]));
        }
    } else if (strncmp(command, "delay", 6) == 0) {
        if (number_of_args < 3) {
            sprintf("delay: bad args", output);
        } else {
            int count = 0;
            while(args_top[1][count] != 0x00) {
                count++;
            }
            int index_args_top2 = 6 + count;
            int len_args_array = MAX_ARGS * ARG_LENGTH;
            int len_from_args_top2 = len_args_array - index_args_top2;
            null_to_space(args_top[2], len_from_args_top2);
            for (int i = index_args_top2; i < len_args_array; i++) {
                if (args_top[2][i] == 0x20 && args_top[2][i+1] == 0x20) {
                    args_top[2][i] = 0x00;
                }
            }

            puts_serial("delay\n");
            puts_serial(args_top[2]);
            puts_serial("\n");
            
            delay(demstr_to_u64(args_top[1]), args_top[2]);
        }
    } else if (strncmp(command, &null_char, 1) == 0) {
        sprintf("no input", output);
    } else {
        sprintf("bad command", output);
    }
}

void writelines(char *output)
{
    if (output[0] == 0x00) {
        putstr(text_x, text_y, white, black, vinfo_global, " ");
    } else {
        putstr(text_x, text_y, white, black, vinfo_global, output);
    }
    text_x = 0;
    text_y += 16;
}

void console(void)
{
    struct ring_buf_char text_buf = gen_buf_char();
    char args_array[MAX_ARGS * ARG_LENGTH] = {0},
        *args_top[MAX_ARGS] = {0}, output[OUTPUT_LENGTH] = {0};

    while (1) {
        readline_serial(&text_buf);
        parse_line(&text_buf, args_array, args_top);
        do_command(args_array, args_top, output);
        writelines(output);

        flush_buf_char(&text_buf);
        flush_array_char(output);
        flush_array_char(args_array);
        flush_array_char(args_top);
        flush_array_char(readline_buf);
    }
}

