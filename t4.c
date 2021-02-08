#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
  pid_t pid, ppid;
  int a = 0;

  pid = fork();
  if (pid == -1){
	printf("Exeption");
  }else if (pid == 0){
	a=a+1;
	pid  = getpid();
        ppid = getppid();
	printf("My pid = %d, my ppid = %d, result = %d \n", (int)pid, (int)ppid, a);
	execl("/usr/bin/printf", "/usr/bin/printf", "new program\n", NULL);
  }else {
	pid  = getpid();
        ppid = getppid();
	
 	printf("My pid = %d, my ppid = %d, result = %d\n", (int)pid, (int)ppid, a);
  }
  return 0;
}
