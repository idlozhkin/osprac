#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[])
{
  printf("argv array:");
  for( int i = 0; i < argc; i++)
  {	
	printf("\nargv[%d] = ",i); 
	printf(argv[i]);
  }

  printf ("\nenvp array : \n");

  for(int i=0; i< sizeof(*envp); i++)
  {
	printf("envp[%d] = %s \n", i, envp[i]);
  }

  return 0;
}
