#include <device/device.h>
#include <task/thread.h>
#include <mm/memory.h>

/** threadsについて
 * スレッドの配列がそのままランキュー
 */
struct thread *threads[THREAD_NUM];
int current_thread_index = 0;

static struct thread empty_thread;
// スタックの大きさを定義
#define STACK_LENGTH    0x1000

/** 周期割り込みの設定
 * timer_period: 周期割り込みの周期
 * previous_interrupt: 前回の周期割り込みの時間
*/
uint64_t timer_period = 0;
uint64_t previous_interrupt = 0;

/** スレッドの生成
 * thread構造体の初期化とスタックの初期化を行う
 */
struct thread thread_gen(void (*func)(int, char**), int argc, char **argv)
{
    struct thread thread;

    thread.stack = (uint64_t *)minmalloc(STACK_LENGTH);
    thread.rsp = (uint64_t)(thread.stack + STACK_LENGTH);
    thread.rip = (uint64_t)thread_exec;
    thread.func_info.func = func;
    thread.func_info.argc = argc;
    thread.func_info.argv = argv;

    put_str_num_serial("thread stack bottom: ", (uint64_t)thread.stack + STACK_LENGTH);
    put_str_num_serial("thread rsp: ", thread.rsp);

    return thread;
}

void thread_stack_init(struct thread *thread)
{
    thread->rsp = init_stack2(thread->rsp, thread->rip, thread);
    put_str_num_serial("thread stack bottom: ", (uint64_t)thread->stack + STACK_LENGTH);
    put_str_num_serial("thread rsp: ", thread->rsp);
}

/** スレッドの実行
 * threadsにスレッドを登録し、スレッドのメイン関数を実行
 * スタックに積まれた初期レジスタをpopし、
 * スタックの底に積んであるメイン関数のアドレスをもとにret命令でメイン関数に移行
 */
void thread_run(struct thread *thread)
{
    // threadsにthreadの登録をする
    int i;
    for (i = 0; i < THREAD_NUM; i++) {
        // threadsのi番目が開いていたら...
        if (threads[i]->state == DEAD) {
            threads[i] = thread; // スレッドをi番目に入れて
            threads[i]->state = RUNNABLE; // stateはRUNNABLE
            threads[i]->index = i; // indexを登録
            break;
        }
    }
}

void thread_exec(struct thread *thread)
{
    thread->func_info.func(thread->func_info.argc, thread->func_info.argv);
    thread_end(thread->index);
    minfree(thread->stack);
    thread_scheduler(0);
}

/** スレッドの終了
 * stateをDEADにし、スケジューラに切り替える
 */
void thread_end(int thread_index)
{
    threads[thread_index]->state = DEAD; // stateはDEADにする
}

/** threadsの初期化処理
 * DEADのスレッドを作ってそれで埋めておく
 */
void threads_init(void)
{
    empty_thread.stack = 0;
    empty_thread.state = DEAD;
    for (int i = 0; i < THREAD_NUM; i++) {
        threads[i] = &empty_thread;
    }
}

void schedule_period_init(uint64_t milli_sec)
{
    timer_period = milli_sec;
}

/** This is thread scheduler
 * This function dumps current and next thread info
 * and calls switch_context
 */
void thread_scheduler(uint64_t old_rip)
{
    // update current_thread_index
    int old_thread_index = current_thread_index;
    // current_thread_index = (current_thread_index + 1) % THREAD_NUM;
    int i = 1;
    while (current_thread_index == old_thread_index) {
        if (threads[(current_thread_index + i) % THREAD_NUM]->state == RUNNABLE) {
            current_thread_index = (current_thread_index + i) % THREAD_NUM;
        }
        i++;
    }

    // save previous thread's rip to struct thread
    threads[old_thread_index]->rip = old_rip;
    put_str_num_serial("threads[old_thread_index].rip: ", threads[old_thread_index]->rip);
    put_str_num_serial("next thread index: ", (uint64_t)current_thread_index);
    put_str_num_serial("next start rsp: ", threads[current_thread_index]->rsp);
    put_str_num_serial("next start func address: ", (uint64_t)(threads[current_thread_index]->func_info.func));
    put_str_num_serial("next thread rip: ", threads[current_thread_index]->rip);
    puts_serial("\n");
    puts_serial("dispatch start\n\n");

    switch_context(&threads[old_thread_index]->rsp, threads[current_thread_index]->rsp);
}
