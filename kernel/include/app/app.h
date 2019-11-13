#pragma once
#include <stdint.h>

#define MAX_ARGS 16
#define ARG_LENGTH 64
#define OUTPUT_LENGTH 64
#define READLINE_BUF_LENGTH 64

extern volatile int readline_flag;
extern volatile char readline_buf[READLINE_BUF_LENGTH];

void console(int argc, char **argv);

