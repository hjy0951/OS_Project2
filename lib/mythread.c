#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>

#include "mythread.h"

static enum mythread_scheduling_policy sched_policy = MYTHREAD_RR;

// Thread control block
struct tcb {
  // TODO: Fill your own code
  int finished; // 0이면 끝나지 않음, 1이면 끝남.
};

static struct tcb* tcbs[1024];
static int n_tcbs = 0;
static int current_thread_id = 0;
static struct itimerval time_quantum;
static struct sigaction ticker;

static int nextTcb() {
  if (sched_policy == MYTHREAD_RR) {
    // TODO: Fill your own code
  } else if (sched_policy == MYTHREAD_FIFO) {
    // TODO: Fill your own code
  }
}

static void tick(int sig) {
  // TODO: Implement your own code

  // HINT: 이 함수에서 nextTcb() 함수를 통해 scheduling 하고, 해당 schedule 함수를 실행.
}

void mythread_init(enum mythread_scheduling_policy policy)
{
  sched_policy = policy;

  // TODO: Implement your own code
  
  // HINT: 이 함수에서 tick 에 대해서 timer signal를...
}

int mythread_create(void (*stub)(void*), void* args)
{
  int tid = -1; // Thread ID

  // TODO: Implement your own code

  return tid;
}

void mythread_join(int tid)
{
  // TODO: Implement your own code
}

