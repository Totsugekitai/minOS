#include <stdint.h>
#include <core/global_variables.h>
#include <device/device.h>
#include <command/command.h>
#include <types/boottypes.h>
#include <graphics/graphics.h>
#include <util/util.h>
#include <app/app.h>

// struct task_queue task_q;
// volatile int readline_flag = 0;
// volatile char readline_buf[READLINE_BUF_LENGTH] = {0};

char args_array[MAX_ARGS * ARG_LENGTH] = {0},
    *argv[MAX_ARGS] = {0}, output[OUTPUT_LENGTH] = {0};
uint8_t keycode, oldkeycode, shift = 0;
uint32_t text_x = 0, text_y = 0;
struct ring_buf_char text_buf;

void readline_serial(void)
{
    keycode = 0x00; // 初期化
    putstr(text_x, text_y, green, black, vinfo_global, "totsugeki@minOS $ ");
    text_x += 8 * 18;
    while (keycode != 0x0d && keycode != 0x0a) { // 改行が来たらやめる
        keycode = 0x00;
        wait_serial_input();
        //keycode = read_serial();
        if (keycode == 0x0d || keycode == 0x0a) {
            continue;
        }
        if (!buf_char_isfull(&text_buf)) {
            enqueue_char(&text_buf, keycode);
            // 文字描画部
            putnum(300, 100, white, black, vinfo_global, keycode);
            putchar(text_x, text_y, white, black, vinfo_global, keycode);
            text_x += 8;
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

void parse_line(void)
{
    // バッファに入っている文字をargs_arrayに移動
    int char_count = 0;
    while (!buf_char_isempty(&text_buf)) {
        dequeue_char(&text_buf, &args_array[char_count]);
        char_count++;
    }
    
    puts_serial("args_array: ");
    puts_serial(args_array);
    puts_serial("\n");
    
    // args_arrayに入った文字を解析
    //char_count++;
    int i, j = 0;
    argv[j] = args_array;
    j++;
    for (i = 0; i < char_count; i++) {
        if (args_array[i] == 0x00) {
            break;
        }
        if (args_array[i] == 0x20) { // スペースだったら
            argv[j] = &args_array[i + 1]; // args_topに次の文字のアドレスを登録
            args_array[i] = 0x00; // スペースをnull文字に置換
            j++;
        }
    }
    // 最後の改行文字を置き換え
    // args_array[i] = 0x00;

    // debug用
    for (int i = 0; i < MAX_ARGS; i++) {
        if (argv[i] != 0x00) {
            puts_serial("[");
            puts_serial(argv[i]);
            puts_serial("]");
            puts_serial("\n");
        } else {
            puts_serial("[]\n");
        }
    }
}

void do_command(void)
{
    // コマンドはargs_arrayの先頭
    char *command = argv[0];
    
    // コマンド引数の数を解析
    int argc = 0;
    for (int i = 0; i < MAX_ARGS; i++) {
        if (argv[i] != 0x00) {
            argc++;
        }
    }
    
    // コマンドによって実行コマンドを振り分ける
    if (strncmp(command, "echo", 5) == 0) {
        echo(argc, argv);
    } else if (strncmp(command, "uptime", 7) == 0) {
        uptime(argc, argv);
    } else if (strncmp(command, "sleep", 6) == 0) {
        sleep(argc, argv);
    } else if (strncmp(command, "", 1) == 0) {
        sprintf("no input", output);
    } else {
        sprintf("bad command", output);
    }
}

void writelines()
{
    if (output[0] == 0x00) {
        putstr(text_x, text_y, white, black, vinfo_global, " ");
    } else {
        putstr(text_x, text_y, white, black, vinfo_global, output);
    }
    text_x = 0;
    text_y += 16;
}

void console_kai(void)
{
    text_buf = gen_buf_char();


    while (1) {
        readline_serial();
        parse_line();
        do_command();
        writelines();

        flush_buf_char(&text_buf);
        flush_array_char(output);
        flush_array_char(args_array);
        flush_argv(argv);
        // flush_array_char(readline_buf);
    }
}

