#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

/*
В родительском процессе Z(s) ...code... A(s,2)
В дочернем процессе D(s, -1) ...code... D(s, -1)

родительский процесс запускается, и работает, увеличивая после завершения значение
семафора на 2,после чего он снова не запустится, пока не станет значение семафора 0

если дочерний процесс запустится до завершения первого, то значение семафора будет 0 
и при уменьшении на 1 процесс заморозится, но он продолжит работу после завершения 
первого процесса, и отработав установит значение семафора 0

после чего процессы придут в ситуацию аналогичную начальной, а значит передача информации через один pipe не нарушится и повторится до окончания цикла
*/

int main()
{
  int     fd[2], result, N, semid;
  size_t size;
  char  resstring[14] = "Hello, world!";
  char  pathname[] = "05-3.c";
  key_t key;
  struct sembuf mybuf;
  mybuf.sem_num = 0;
  mybuf.sem_flg = 0;

  if ((key = ftok(pathname, 0)) < 0){
    printf("Cant generate key\n");
    exit(-1);
  }

  if (( semid = semget(key, 1, 0666 | IPC_CREAT)) < 0){
    printf("Cant generate key\n");
    exit(-1);
  }

  if (pipe(fd) < 0) {
    printf("Can\'t open pipe\n");
    exit(-1);
  }


  scanf("%d", &N);
  result = fork();

  for (int i =0; i< N; i++){
  if (result < 0) {
    printf("Can\'t fork child\n");
    exit(-1);
  } else if (result > 0) {

   /* Parent process */

   mybuf.sem_op = 0;
   if (semop(semid, &mybuf, 1) < 0){
     exit(-1);
   }
    if (i>0){
      size = read(fd[0], resstring, 14);
      resstring[12] = '!';
    }

    size = write(fd[1], resstring, 14);

    printf("Parent exit, with message %s\n", resstring);

    mybuf.sem_op = 2;
    if (semop(semid, &mybuf, 1) < 0){
      exit(-1);
    }

  } else {

    /* Child process */

    mybuf.sem_op = -1;
    if (semop(semid, &mybuf, 1) < 0){
      exit(-1);
    }
 
    size = read(fd[0], resstring, 14);

    resstring[12] = '?';
    size = write(fd[1], resstring, 14);
    printf("Child exit, resstring:%s\n", resstring);

    mybuf.sem_op = -1;
    if (semop(semid, &mybuf, 1) < 0){
      exit(-1);
    }
  }
  }
    if (close(fd[0]) < 0) {
      printf("Can\'t close reading side of pipe\n"); exit(-1);
    }
    if (close(fd[1]) < 0) {
      printf("Can\'t close reading side of pipe\n"); exit(-1);
    }
  return 0;
}
