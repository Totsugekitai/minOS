#include <device/device.h>
#include <task/thread.h>

int thread_counter = 0;

/** threadsについて
 * スレッドの配列がそのままランキュー
 * 0番目は必ずスレッドスケジューラのスレッドにする
 */
struct thread threads[THREAD_NUM];
int current_thread_index = 0;

// スタックはとりあえず固定配置
uint64_t stack0[0x1000];
uint64_t stack1[0x1000];
uint64_t stack2[0x1000];
uint64_t stack3[0x1000];
uint64_t stack4[0x1000];
uint64_t stack5[0x1000];

/** 周期割り込みの設定 
 * timer_period: 周期割り込みの周期
 * previous_interrupt: 前回の周期割り込みの時間
*/
uint64_t timer_period = 0;
uint64_t previous_interrupt = 0;

/** スレッドの生成
 * thread構造体の初期化とスタックの初期化を行う
 */
struct thread thread_gen(uint64_t *stack, void (*func)(int, char**),
    int argc, char **argv)
{
    struct thread thread;

    thread.stack = stack;
    thread.rsp = (uint64_t)(stack + 0x1000);
    thread.func_info.func = func;
    thread.func_info.argc = argc;
    thread.func_info.argv = argv;

    thread.rsp = stack_init(thread.rsp, func);

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
    // // threads[SCHED_THREAD_INDEX]はスレッドスケジューラなのでこれでよい
    // save_and_dispatch(threads[thread_index].rsp, threads[SCHED_THREAD_INDEX].rsp);
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
 * 各スレッドにタイムスライスを持たせてそれが尽きた or 処理が終了したら次のスレッドを実行したい
 * save_and_dispatchで今実行しているスレッドのコンテキストを保存し、次のスレッドをディスパッチ
 */
void thread_scheduler(void)
{
    int next_thread_index = 0;
    if (current_thread_index == 1) {
        current_thread_index = 0;
        next_thread_index = 0;
    } else {
        current_thread_index = 1;
        next_thread_index = 1;
    }
    // for (int i = 1; i < THREAD_NUM; i++) {
    //     if (threads[(current_thread_index + i) % THREAD_NUM].state == RUNNABLE) {
    //         next_thread_index = (current_thread_index + i) % THREAD_NUM;
    //         break;
    //     }
    // }
    puts_serial("next thread index: ");
    putnum_serial((uint64_t)next_thread_index);
    puts_serial("\n");

    puts_serial("start rsp: ");
    putnum_serial(threads[next_thread_index].rsp);
    puts_serial("\n");

    // dispatch(threads[next_thread_index].rsp);
    if (thread_counter > 0) {
        puts_serial("dispatch2 start\n\n");
        save_and_dispatch2(&(threads[current_thread_index].rsp), threads[next_thread_index].rsp);
    } else {
        thread_counter++;
        puts_serial("counter: ");
        putnum_serial((uint64_t)thread_counter);
        puts_serial("\n");
        puts_serial("dispatch start\n\n");
        save_and_dispatch(&(threads[current_thread_index].rsp), threads[next_thread_index].rsp);
    }
}
