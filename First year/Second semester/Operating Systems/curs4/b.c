#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv) {

	int pid;

	printf("blabla\n");
	pid = fork();

	if(pid == 0) {
		printf("Child specific code\n");
		exit(0);
	}

	printf("Parent specific code\n");
	wait(0);
	return 0;

// fork returns 0 in the child, returns child PID in the parent!
}
