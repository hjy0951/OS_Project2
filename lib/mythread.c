#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>

#include "mythread.h"

#include <stdlib.h>

static enum mythread_scheduling_policy sched_policy = MYTHREAD_RR;

// Thread control block
struct tcb {
  // TODO: Fill your own code
  int finished; // 0이면 끝나지 않음, 1이면 끝남.
  int t_id;
  ucontext_t* ucp;
};

static struct tcb* tcbs[1024]; // Q?
static int n_tcbs = 0; //ther number of active tcb
static int current_thread_id = 0; // tcb[current_thread_id], active thread
static struct itimerval time_quantum; //preemtive, RR, 5ms
static struct sigaction ticker; // finish time quantum -> tick

static int nextTcb() {
  if (sched_policy == MYTHREAD_RR) {
    // TODO: Fill your own code
    return current_thread_id+1;
  }
  else if (sched_policy == MYTHREAD_FIFO) {
    // TODO: Fill your own code
    if((tcbs[current_thread_id] -> finished == 1)
     && (current_thread_id < n_tcbs))
      //current_thread_id+1 context start
      return current_thread_id+1;
  }
}

//signal handler?
static void tick(int sig) {
  // TODO: Implement your own code
  printf("tick\n");
  current_thread_id = nextTcb();
  // HINT: 이 함수에서 nextTcb() 함수를 통해 scheduling 하고, 해당 schedule 함수를 실행.
}

void mythread_init(enum mythread_scheduling_policy policy)
{
  sched_policy = policy;

  // TODO: Implement your own code
  //timer
  memset(&ticker,0,sizeof(ticker));

  ticker.sa_handler = &tick;

  sigaction(SIGVTALRM,&ticker,NULL);

  time_quantum.it_value.tv_sec = 0;
  time_quantum.it_value.tv_usec = 200000;

  time_quantum.it_interval.tv_sec = 0;
  time_quantum.it_interval.tv_usec = 300000;

  setitimer(ITIMER_VIRTUAL, &time_quantum, NULL);
  //
  // getcontext(tcbs[0]->ucp);  
  // HINT: 이 함수에서 tick 에 대해서 timer signal를...
}

//
void new_stub(void (*stub)(void*), void* args){
  stub(args);
  tcbs[current_thread_id] -> finished = 1;
}
//

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

