#include <task/thread.h>

/** スレッドの生成
 * 初期タイムスライスはINIT_TIME_SLICEで決めておく
 */
struct thread thread_gen(uint64_t *stack, uint64_t *func,
    int argc, char **argv)
{
    struct thread thread;

    thread.stack = stack;
    thread.timeslice = INIT_TIME_SLICE;
    thread.func_info.func = func;
    thread.func_info.argc = argc;
    thread.func_info.argv = argv;

    return thread;
}

/** スレッドの実行
 * threadsにスレッドを登録し、スレッドのメイン関数を実行
 */
void thread_run(struct thread thread)
{
    // threadsにthreadの登録をする
    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        // threadsのi番目が開いていたら...
        if (threads[i].state == DEAD) {
            threads[i] = thread; // スレッドをi番目に入れて
            threads[i].state = RUNNABLE; // stateはRUNNABLE
            break;
        }
    }

    int thread_index = i;

    // スレッドのメイン関数を実行する
    void (*func)(int, char**) = threads[thread_index].func_info.func;
    (*func)(threads[i].func_info.argc, threads[thread_index].func_info.argv);

    // スレッドを終了する
    thread_end(thread_index);
}

/** スレッドの終了
 * stateをDEADにし、スケジューラに切り替える
 */
void thread_end(int thread_index)
{
    threads[thread_index].state = DEAD; // stateはDEADにする
    // threads[SCHED_THREAD_INDEX]はスレッドスケジューラなのでこれでよい
    save_and_dispatch(&(threads[thread_index].rsp), &(threads[SCHED_THREAD_INDEX].rsp));
}

/** threadsの初期化処理
 * DEADのスレッドを作ってそれで埋めておく
 */
void threads_init(void)
{
    struct thread empty_thread;
    empty_thread.stack = 0;
    empty_thread.timeslice = 0;
    empty_thread.state = DEAD;
    for (int i = 0; i < THREAD_NUM; i++) {
        threads[i] = empty_thread;
    }
}

/** スレッドスケジューラ
 * 各スレッドにタイムスライスを持たせてそれが尽きた or 処理が終了したら次のスレッドを実行したい
 * 疑問：タイムスライスの更新はどうする？
 * 疑問：タイムスライスが尽きたときのスケジューラ移行処理の書き方がわからん
 */
void thread_scheduler(uint64_t milli_interval)
{
    while (1) {
        int old_thread_index = current_thread_index;
        current_thread_index = search_next_thread();
        save_and_dispatch(&(threads[old_thread_index].rsp), &(threads[current_thread_index].rsp));
    }
}

/** 次に実行するスレッドを探す
 * 戻り値は次に実行するスレッドのインデックス
 */
int search_next_thread(void)
{
    int next = 0;
    uint64_t max_timeslice = 0;
    for (int i = 0; i < THREAD_NUM; i++) {
        if (threads[i].timeslice > max_timeslice) {
            max_timeslice = threads[i].timeslice;
            next = i;
        }
    }
    return next;
}