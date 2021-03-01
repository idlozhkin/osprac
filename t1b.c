#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  int     *array;    
  int     shmid;   
  int     new = 1;   
  char    pathname[] = "t1a.c";
  char    str[11];
  key_t   key;       
  
  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((shmid = shmget(key, 11*sizeof(char), 0666|IPC_CREAT|IPC_EXCL)) < 0) {
    if (errno != EEXIST) {
      printf("Can\'t create shared memory\n");
      exit(-1);
    } else {
      if ((shmid = shmget(key, 11*sizeof(char), 0)) < 0) {
        printf("Can\'t find shared memory\n");
        exit(-1);
      }
      new = 0;
    }
  }

  if ((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
    printf("Can't attach shared memory\n");
    exit(-1);
  }

    for(size_t i = 0; i<11; i++){
	str[i]=array[i];
  }

  printf("String: %s \n", str);

  if (shmdt(array) < 0) {
    printf("Can't detach shared memory\n");
    exit(-1);
  }
	
  if (shmctl(shmid, IPC_RMID, NULL)<0){
 	printf("Cant delete memory\n");
  }
  return 0;
}
