#include <unistd.h>
#include <stdio.h>

int main(){
	
	printf("UID- %d\n", getuid());
	printf("Group ID- %d\n", getgid());
	
	return 0;
}
