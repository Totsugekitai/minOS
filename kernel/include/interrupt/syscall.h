#pragma once
#include <stdint.h>
void do_syscall(int syscall_id, uint64_t *params);
