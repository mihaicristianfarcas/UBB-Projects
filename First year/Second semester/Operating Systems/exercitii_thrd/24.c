#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
	int id;
	int *pipe;
}data;

typedef struct {
	int id, nr1, nr2;
}response;

void* func(void* arg) {
	data d = *((data*)arg);

	int nr1 = random() % 100 + 1;
	int nr2 = random() % 100 + 1;

	printf("ID: %d; Nr. 1: %d, Nr. 2: %d\n", d.id, nr1, nr2);

	response r;
	r.id = d.id;
	r.nr1 = nr1;
	r.nr2 = nr2;

	pthread_mutex_lock(&m);
	if(write(d.pipe[1], &r, sizeof(response)) == 0) {
		perror("Error on write\n");
	}
	pthread_mutex_unlock(&m);

	return NULL;
}

int main(int argc, char* argv[]) {

	if(argc != 2) {
		printf("Usage: N\n");
		exit(1);
	}

	int t2c[2];
	if(pipe(t2c) != 0) {
		perror("Error creating pipe.\n");
		exit(2);
	}

	int n = atoi(argv[1]);

	int f = fork();
	if(f < 0) {
		perror("My child...\n");
		exit(3);
	} else if(f == 0) {
		close(t2c[1]);
		
		for(int i = 0; i < n; i++) {
			response r;
			
			if(read(t2c[0], &r, sizeof(response)) == 0) {
				perror("Error on read.\n");
				exit(4);
			}
			
			int avg = (r.nr1 + r.nr2) / 2;
			printf("ID: %d; Average: %d\n", r.id, avg);
		}
		
		close(t2c[0]);
		exit(0);		
	} else {
		close(t2c[0]);
		
		pthread_t th[n];
		for(int i = 0; i < n; i++) {
			data* d = (data*)malloc(sizeof(data));
			d->id = i;
			d->pipe = t2c;
			if(pthread_create(&th[i], NULL, func, d) != 0) {
				perror("Error creating thread.\n");
				exit(5);
			}
		}

		for(int i = 0; i < n; i++) {
			pthread_join(th[i], NULL);
		}

		close(t2c[1]);
		pthread_mutex_destroy(&m);
		wait(0);
	}

	return 0;
}
