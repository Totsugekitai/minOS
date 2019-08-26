#pragma once
#include <stdint.h>
#include <types/boottypes.h>

void echo(char *str, char *output);
void uptime(char *output);
void sleep(uint64_t time);
void delay(uint64_t time, char *command_line);

