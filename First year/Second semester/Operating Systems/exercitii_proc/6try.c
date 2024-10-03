#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>

int main(int argc, char** argv) {

	int p2c[2], c2p[2];
	pipe(p2c); pipe(c2p);

	int f = fork();

	if(f == 0) {
		close(p2c[1]);
		close(c2p[0]);

		int n, nr;
	        float avg = 0;
		read(p2c[0], &n, sizeof(int));

		for(int i = 0; i < n; i++) {
			read(p2c[0], &nr, sizeof(int));
			avg += nr;
		}

		avg /= n;
		write(c2p[1], &avg, sizeof(int));

		close(c2p[1]);
		close(p2c[0]);
		exit(0);
	}
	
	close(p2c[0]);
	close(c2p[1]);

	int n, nr;
	float rez = 0;

	printf("n = ");
	scanf("%d", &n);

	write(p2c[1], &n, sizeof(int));
	srandom(time(NULL));
	
	for(int i = 0; i < n; i++) {
		nr = random() % 10;
		printf("Generated nr: %d\n", nr);
		write(p2c[1], &nr, sizeof(int));
	}

	wait(0);
	read(c2p[0], &rez, sizeof(int));
	printf("Average: %d\n", n);
	
	close(p2c[1]);
	close(c2p[0]);
	return 0;
}
