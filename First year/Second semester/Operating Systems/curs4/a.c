#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
	printf("before %d %d\n", getpid(), getppid());
	fork();
	printf("after %d %d\n", getpid(), getppid());
	return 0;

// fork returns 0 in the child, child PID in the parent!
}
