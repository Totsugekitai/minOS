#include <stdint.h>
#include <command/command.h>
#include <graphics/graphics.h>
#include <util/util.h>

#define MAX_ARGS    10

extern struct pix_format white;
extern struct pix_format black;

uint32_t text_x = 0, text_y = 0;

void console(char *str, struct video_info *vinfo)
{
    uint8_t args_count = 0, arg_head_mem = 0;
    uint32_t i;
    char args_head[MAX_ARGS]; // 引数の先頭が何番目に来るかを記録しておく配列

    for (i = 0; i < MAX_ARGS; i++) {
        args_head[i] = -1; // 何も入っていない部分は-1
    }

    // 単語の数を解析し、スペースと改行をヌル文字に置き換える
    while (str[i] != 0x00) { // 末尾のヌル文字の手前まで見る
        if (str[i] == ' ' || str[i] == 0x0a) { // スペースか改行
            args_head[args_count] = arg_head_mem;
            args_count++;
            arg_head_mem = i + 1;
            str[i] = 0x00;
        }
        i++;
    }

    // strの１つ目の単語とコマンドの照合を行う
    if (comptext(str[args_head[0]], "echo")) { // echoコマンド
        if (args_head[1] != -1 && args_head[2] == -1) {
            echo(args_head[1], vinfo);
        } else {
            putstr(text_x, text_y, white, black, vinfo, "argument error.");
        }
    } else {
        putstr(text_x, text_y, white, black, vinfo, "command not found.");
    }
    
    // 最後に文字列を改行して終わる
    text_x = 0;
    text_y += 16;
}

