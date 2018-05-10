#include <stdio.h>

#include "mythread.h"

void f(void* args)
{
  fprintf(stderr, "f\n");
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
  mythread_init(MYTHREAD_RR);

  int tid1 = mythread_create(f, NULL);

  int n = 5;
  int tid2 = mythread_create(g, &n);

  mythread_join(tid1);
  mythread_join(tid2);

  return 0;
}
