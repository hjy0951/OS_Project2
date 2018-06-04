#include <stdio.h>

#include "mythread.h"

void f(void* args)
{
  const int n = *((int*)args);

  for (int i = 0; i < n; ++i) {
    fprintf(stderr, "f\n");
  }
}

void g(void* args)
{
  const int n = *((int*)args);

  for (int i = 0; i < n; ++i) {
    fprintf(stderr, "g\n");
  }
 }

int main()
{
  printf("start init\n");
  // mythread_init(MYTHREAD_FIFO);
  mythread_init(MYTHREAD_RR);
  printf("finish init\n");

  int m = 10;
  printf("\nstart f1\n");
  int tid1 = mythread_create(f, &m);
  printf("finish f1\n");

  int n = 10;
  printf("\nstart f2\n");
  int tid2 = mythread_create(g, &n);
  printf("finish f2\n");

  mythread_join(tid1);
  mythread_join(tid2);

  return 0;
}
