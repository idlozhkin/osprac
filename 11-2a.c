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
  char pathname[]="11-2a.c";
  key_t  key;
  int i,len, maxlen;

  struct mymsgbuf
  {
    long mtype;
    struct {
	short sinfo;
	float finfo;
    } info;
  } mybuf;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  for (i = 1; i <= 5; i++) {
    mybuf.mtype = 1;
    mybuf.info.sinfo = 12;
    mybuf.info.finfo = 1.2;
    len = sizeof(mybuf.info);

    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }
  }
  
  maxlen = sizeof(mybuf.info);

    for (i = 1; i <= 5; i++) {
    if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 0, 0)) < 0) {
      printf("Can\'t receive message from queue\n");
      exit(-1);
    }

    printf("2 programm message type = %ld, sinfo = %i, finfo = %f\n", mybuf.mtype, mybuf.info.sinfo, mybuf.info.finfo);
  }



  return 0;
}
