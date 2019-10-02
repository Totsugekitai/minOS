#include <task/thread.h>

/* スレッドの生成 */
struct thread thread_gen(uint64_t *stack, uint64_t *func,
    int argc, char **argv)
{
    struct thread thread;

    thread.thread_id = thread_number;

    thread_number++; // スレッドの生成カウンタをインクリメント

    thread.stack = stack;
    thread.func_info.func = func;
    thread.func_info.argc = argc;
    thread.func_info.argv = argv;

    return thread;
}

/* スレッドの実行 */
void thread_run(struct thread thread)
{
    // threadsにthreadの登録をする
    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        // threadsのi番目が開いていたら...
        if (threads[i].state == DEAD) {
            threads[i] = thread; // スレッドをi番目に入れて
            threads[i].state = LIVE; // stateはLIVE
            break;
        }
    }

    int thread_id = threads[i].thread_id;

    // スレッドのメイン関数を実行する
    void (*func)(int, char**) = threads[i].func_info.func;
    (*func)(threads[i].func_info.argc, threads[i].func_info.argv);

    // スレッドを終了する
    thread_end(thread_id);
}

/* スレッドの終了 */
void thread_end(int thread_id)
{
    for (int i = 0; i < THREAD_NUM; i++) {
        if (threads[i].thread_id == thread_id) {
            threads[i].state =DEAD; // stateをDEADにする
            break;
        }
    }
}

void threads_init(void)
{
    struct thread empty_thread;
    empty_thread.thread_id = -1;
    empty_thread.stack = 0;
    empty_thread.state = DEAD;
    for (int i = 0; i < THREAD_NUM; i++) {
        threads[i] = empty_thread;
    }
}

/* スレッドスケジューラ */
void thread_scheduler(uint64_t milli_interval)
{
    uint64_t next_switch_timing = milli_clock + milli_interval;
    if (next_switch_timing < milli_clock) {
        if (current_thread_index == THREAD_NUM - 1)
        {
            save_and_dispatch(&(threads[current_thread_index].rsp), &(threads[0].rsp));
            current_thread_index = 0;
        } else {
            save_and_dispatch(&(threads[current_thread_index].rsp),
                              &(threads[current_thread_index + 1].rsp));
            current_thread_index++;
        }
        next_switch_timing = milli_clock + milli_interval;
    }
}