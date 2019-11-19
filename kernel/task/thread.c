#include <device/device.h>
#include <task/thread.h>

/** threadsについて
 * スレッドの配列がそのままランキュー
 * 0番目は必ずスレッドスケジューラのスレッドにする
 */
struct thread threads[THREAD_NUM];
int current_thread_index = 0;

// スタックの大きさを定義
#define STACK_LENGTH    0x1000

// スタックはとりあえず固定配置
uint64_t stack0[STACK_LENGTH];
uint64_t stack1[STACK_LENGTH];
uint64_t stack2[STACK_LENGTH];
uint64_t stack3[STACK_LENGTH];
uint64_t stack4[STACK_LENGTH];
uint64_t stack5[STACK_LENGTH];

/** 周期割り込みの設定
 * timer_period: 周期割り込みの周期
 * previous_interrupt: 前回の周期割り込みの時間
*/
uint64_t timer_period = 0;
uint64_t previous_interrupt = 0;

/** スレッドの生成
 * thread構造体の初期化とスタックの初期化を行う
 */
struct thread thread_gen(uint64_t *stack, uint64_t *func,
    int argc, char **argv)
{
    struct thread thread;

    thread.stack = stack;
    thread.rsp = (uint64_t)(stack + STACK_LENGTH);
    thread.rip = (uint64_t)func;
    thread.func_info.func = func;
    thread.func_info.argc = argc;
    thread.func_info.argv = argv;

    // thread.rsp = init_stack(thread.rsp, (uint64_t)func);
    thread.rsp = init_stack2(thread.rsp, (uint64_t)func);

    puts_serial("thread stack bottom: ");
    putnum_serial((uint64_t)thread.stack + STACK_LENGTH);
    puts_serial("\n");
    puts_serial("thread rsp: ");
    putnum_serial(thread.rsp);
    puts_serial("\n");

    return thread;
}

/** スレッドの実行
 * threadsにスレッドを登録し、スレッドのメイン関数を実行
 * スタックに積まれた初期レジスタをpopし、
 * スタックの底に積んであるメイン関数のアドレスをもとにret命令でメイン関数に移行
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
}

/** スレッドの終了
 * stateをDEADにし、スケジューラに切り替える
 */
void thread_end(int thread_index)
{
    threads[thread_index].state = DEAD; // stateはDEADにする
}

/** threadsの初期化処理
 * DEADのスレッドを作ってそれで埋めておく
 */
void threads_init(void)
{
    struct thread empty_thread;
    empty_thread.stack = 0;
    empty_thread.state = DEAD;
    for (int i = 0; i < THREAD_NUM; i++) {
        threads[i] = empty_thread;
    }
}

void schedule_period_init(uint64_t milli_sec)
{
    timer_period = milli_sec;
}

/** スレッドスケジューラ
 * 現在のripを保存し、次のスレッドをディスパッチ
 */
void thread_scheduler(uint64_t old_rip)
{
    // current_thread_indexを更新
    int old_thread_index = current_thread_index;
    int i = 1;
    while (current_thread_index == old_thread_index) {
        if (threads[(current_thread_index + i) % THREAD_NUM].state == RUNNABLE) {
            current_thread_index = (current_thread_index + i) % THREAD_NUM;
        }
        i++;
    }

    puts_serial("thread_scheduler old_rip: ");
    putnum_serial(old_rip);
    puts_serial("\n");
    // 前のスレッドのripをthread構造体に保存
    threads[old_thread_index].rip = old_rip;
    puts_serial("threads[old_thread_index].rip: ");
    putnum_serial(threads[old_thread_index].rip);
    puts_serial("\n");

    puts_serial("next thread index: ");
    putnum_serial((uint64_t)current_thread_index);
    puts_serial("\n");

    puts_serial("next start rsp: ");
    putnum_serial(threads[current_thread_index].rsp);
    puts_serial("\n");


    puts_serial("next start func address: ");
    putnum_serial((uint64_t)(threads[current_thread_index].func_info.func));
    puts_serial("\n");

    puts_serial("next thread rip: ");
    putnum_serial(threads[current_thread_index].rip);
    puts_serial("\n\n");

    puts_serial("dispatch start\n\n");

    // dispatch(threads[current_thread_index].rsp,
    //     &(threads[old_thread_index].rsp), threads[current_thread_index].rip);
    switch_context(&threads[old_thread_index].rsp, threads[current_thread_index].rsp);
}
