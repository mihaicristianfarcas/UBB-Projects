#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
	
	int a2b[2], b2a[2];
        pipe(a2b);
        pipe(b2a);

	int f = fork();
	if(f == 0) {
		close(a2b[1]);
		close(b2a[0]);

		int n, m, diff, ok = 0;
		
		srandom(time(0));
		n = random() % 901 + 100;
		
		while(!ok) {
			
			read(a2b[0], &m, sizeof(int));
			
			if(n > m) 
				diff = n - m;
			else
				diff = m - n;

			if(diff < 50){
				printf("%d is less than 50!\n", diff);
				ok = 1;
			} 
			else printf("%d >= 50, try again...\n", diff);
		
			write(b2a[1], &ok, sizeof(int));
		}	
	
		close(a2b[0]);
		close(b2a[1]);
		exit(0);
	}
	
	close(a2b[0]);
	close(b2a[1]);

	int n, ok = 0, cnt = 0;
	srandom(time(0));

	while(!ok) {
		n = random() % 1001 + 50;
		cnt++;
		write(a2b[1], &n, sizeof(int));
		read(b2a[0], &ok, sizeof(int));
	}
	
	wait(0);
	printf("Count: %d\n", cnt);
	close(a2b[1]);
	close(b2a[0]);
	return 0;
}
