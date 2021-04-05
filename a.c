#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int a = 0;

void *mythread(void *dummy)
{
  pthread_t mythid;

  mythid = pthread_self();

  a = a+1;
  printf("Thread %u, Calculation result = %d\n", mythid, a);
  return NULL;
}

int main()
{
  pthread_t thid, newthid, mythid;
  int result,newresult;

  result = pthread_create (&thid, (pthread_attr_t *)NULL, mythread, NULL);
  newresult = pthread_create(&newthid, (pthread_attr_t *)NULL, mythread, NULL);

  if (result != 0) {
    printf ("Error on thread create, return value = %d\n", result);
    exit(-1);
  }
  if (newresult != 0) {
    printf ("Error on thread create, return value = %d\n", newresult);
    exit(-1);
  }
  printf("Threads created, thid = %u, newthid = %u\n", thid, newthid);

  mythid = pthread_self();

  a = a+1;

  printf("Thread %u, Calculation result = %d\n", mythid, a);

  pthread_join(thid, (void **)NULL);
  pthread_join(newthid, (void **)NULL);

  return 0;
} 
