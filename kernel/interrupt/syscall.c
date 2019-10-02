#include <stdint.h>
#include <interrupt/syscall.h>

#define SYSCALL_INTERRUPT_NO    0x80

void do_syscall(int syscall_id, uint64_t *params)
{
    switch (syscall_id) {
        case 0:
            thread_run();
            break;
        case 1:
            thread_exit();
            break;
        default:
            break;
    }
    return syscall_id;
}