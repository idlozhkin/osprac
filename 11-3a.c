#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255

int main(void)
{
  int msqid;
  char pathname[]="11-3a.c";
  key_t  key;
  int len, max;
  float num;

  struct myclientmsgbuf
  {
    long mtype;
    struct {
	pid_t pid;
	float mes;
    } info;
  } myclientbuf;

  struct myservermsgbuf
  {
    long mtype;
    struct {
	float mes;
    } info;
  } myserverbuf;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }
  
  myclientbuf.mtype = 1;
  myclientbuf.info.pid = getpid();

  printf("Num = ");
  scanf("%f", &num);
  len = sizeof(myclientbuf.info);
  myclientbuf.info.mes = num;


    if (msgsnd(msqid, (struct myclientmsgbuf *) &myclientbuf, len, 0) < 0) {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }

  max = sizeof(myserverbuf.info);

  if (msgrcv(msqid, &myserverbuf, max,getpid(), 0) < 0) {
    printf("Can\'t receive message to queue\n");
    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
    exit(-1);
  }

  printf("New num = %f\n", myserverbuf.info.mes);

  return 0;
}
