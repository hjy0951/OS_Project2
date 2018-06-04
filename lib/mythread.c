#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>

#include "mythread.h"

#include <stdlib.h>
#include <stdio.h>

static enum mythread_scheduling_policy sched_policy = MYTHREAD_RR;

// Thread control block
struct tcb {
  // TODO: Fill your own code
  int finished; // 0이면 끝나지 않음, 1이면 끝남.
  ucontext_t* ucp;
  int stack[16384];
};

static struct tcb* tcbs[1024]; // Q? / tcbs[0] is a main thread
static int n_tcbs = 0; //ther number of active tcb
static int current_thread_id = 0; // tcb[current_thread_id], active thread
static struct itimerval time_quantum; //preemtive, RR, 5ms
static struct sigaction ticker; // finish time quantum -> tick

// ucontext_t main_context;

static int nextTcb() {
  if (sched_policy == MYTHREAD_RR) {
    // TODO: Fill your own code
    int nextTcb;
    if(n_tcbs == 0)
      return 0;
    else{
      nextTcb = (current_thread_id+1)%(n_tcbs+1);
      while(tcbs[nextTcb]->finished == 1)
        nextTcb = (nextTcb+1)%(n_tcbs+1);
      return nextTcb;
    }
  }
  else if (sched_policy == MYTHREAD_FIFO) {
    // TODO: Fill your own code
    //?
  }
}

//signal handler
static void tick(int sig) {
  // TODO: Implement your own code
  // printf("\ntick\n");
  if(sched_policy == MYTHREAD_RR){
    //context switchingint prev_thread_id = current_thread_id;
    int prev_thread_id = current_thread_id;
    // printf("prev id : %d\n", prev_thread_id);
    current_thread_id = nextTcb();
    // printf("current id : %d\n", current_thread_id);
    if(prev_thread_id == current_thread_id)
      return;
    else{
      // printf("swap\n");
      swapcontext(tcbs[prev_thread_id]->ucp, tcbs[current_thread_id]->ucp);
    }
  }

  // HINT: 이 함수에서 nextTcb() 함수를 통해 scheduling 하고, 해당 schedule 함수를 실행.
}

void mythread_init(enum mythread_scheduling_policy policy)
{
  sched_policy = policy;

  // TODO: Implement your own code
  //initialize tcbs
  for(int i = 0; i < 1024 ; i++){
    tcbs[i] = (struct tcb*)malloc(sizeof(struct tcb));
    tcbs[i]->finished = 0;
    tcbs[i]->ucp = (ucontext_t*)malloc(sizeof(ucontext_t));

  }

  //timer
  // printf("set timer\n");
  memset(&ticker,0,sizeof(ticker));

  sigemptyset(&ticker.sa_mask);
  ticker.sa_handler = &tick;

  // sigaction(SIGVTALRM,&ticker,NULL);
  sigaction(SIGALRM,&ticker,NULL);

  time_quantum.it_value.tv_sec = 0;
  time_quantum.it_value.tv_usec = 2000;

  time_quantum.it_interval.tv_sec = 0;
  time_quantum.it_interval.tv_usec = 2000;
  // printf("current id : %d\n",current_thread_id);

  // setitimer(ITIMER_VIRTUAL, &time_quantum, NULL);
  setitimer(ITIMER_REAL, &time_quantum, NULL);
  // printf("finish timer setting\n");
  //

  //save main context
  getcontext(tcbs[0]->ucp); 

  if(sched_policy == MYTHREAD_RR)
    printf("\nRR Scheduling\n");
  else
    printf("\nFIFO Scheduling\n");
  // HINT: 이 함수에서 tick 에 대해서 timer signal를...
}

//
void new_stub(void (*stub)(void*), void* args, struct tcb** tcbs, int tid){
  // printf("do stub\n");
  stub(args);
  tcbs[tid] -> finished = 1;
  swapcontext(tcbs[tid]->ucp, tcbs[0]->ucp);
  current_thread_id = 0;
}
//

int mythread_create(void (*stub)(void*), void* args)
{
  // printf("\nthread creating\n");
  int tid = -1; // Thread ID
  // TODO: Implement your own code
  n_tcbs++;
  // printf("n_tcbs : %d\n", n_tcbs);
  tid = n_tcbs;
  // printf("tid : %d\n", tid);

  // printf("getcontext\n");
  getcontext(tcbs[tid]->ucp); //save new thread context
  // printf("save context\n");

  // printf("makecontext\n");
  tcbs[tid]->ucp->uc_stack.ss_sp
   = tcbs[tid]->stack;
  tcbs[tid]->ucp->uc_stack.ss_size
   = sizeof(tcbs[tid]->stack);
  tcbs[tid]->ucp->uc_link = &tcbs[0];
  makecontext(tcbs[tid]->ucp, new_stub, 4, stub, args, tcbs, tid);
  // printf("make\n");
  current_thread_id = tid;
  swapcontext(tcbs[0]->ucp, tcbs[tid]->ucp);

  return tid;
}

void mythread_join(int tid)
{
  // printf("join\n");
  int prev = current_thread_id;

  if(prev == tid){
    while(tcbs[current_thread_id]->finished == 0){
      if(tcbs[current_thread_id]->finished == 1)
        break;
    }
  }
  else{
    if(tcbs[tid]->finished == 1){
      return;
    }
    // printf("swap and join\n");
    current_thread_id = tid;
    swapcontext(tcbs[prev]->ucp, tcbs[current_thread_id]->ucp);
    while(tcbs[current_thread_id]->finished == 0){
      if(tcbs[current_thread_id]->finished == 1)
        break;
    }
  }
  // printf("finish join\n");
  
}

