#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct {
	int id, times;
}data;

pthread_mutex_t *checkpoints;
pthread_barrier_t b;

void* fthread(void* a) {
	data *d = (data*)a;
	printf("Thread %d started\n", d->id);
	int n = d->times;

	pthread_barrier_wait(&b);

	while(n--) {
		printf("Thread %d waiting on checkpoint %d\n", d->id, n);

		int entered = 0, i = 0;
		while(entered == 0) {
			if(pthread_mutex_trylock(&checkpoints[i]) == 0) {
				usleep(rand() % 2 * 10000 + 100000);
				entered = 1;
				pthread_mutex_unlock(&checkpoints[i]);

			} else {
				i = (i+1) % d->times;
			}
		}
		
	}

	printf("Thread %d done\n", d->id);
	free(a);
	return NULL;
}

int main(int argc, char* argv[]) {

	if(argc != 3) {
		printf("Usage: N, M\n");
		exit(1);
	}

	pthread_t *th;
	int n = atoi(argv[1]);
	int m = atoi(argv[2]);
	printf("N: %d; M: %d\n", n, m);

	th = (pthread_t*)malloc(sizeof(pthread_t) * n);
	pthread_barrier_init(&b, NULL, n);
	
	checkpoints = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t) * m);
	for(int i = 0; i < m; i++) {
		pthread_mutex_init(&checkpoints[i], NULL);
	}

	for(int i = 0; i < n; i++) {
		data *d = (data*)malloc(sizeof(data));
		d->id = i;
		d->times = m;
		if(pthread_create(&th[i], NULL, fthread,d) < 0) {
			perror("Error creating thread");
			exit(2);
		}
	}

	for(int i = 0; i < n; i++) {
		pthread_join(th[i], NULL);
	}
	
	for(int i = 0; i < m; i++) {
                pthread_mutex_destroy(&checkpoints[i]);
        }
	
	pthread_barrier_destroy(&b);
	return 0;
}
