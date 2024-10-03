#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

sem_t* s;
pthread_barrier_t b;

typedef struct {
	int id, n;
}data;

void* race(void* arg) {
	data d = *((data*)arg);

	printf("Car %d starting!\n", d.id);
	pthread_barrier_wait(&b);

	for(int i = 0; i < d.n; i++) {
		sem_wait(&s[i]);
		printf("Car %d is passing through checkpoint %d!\n", d.id, i);
		sem_post(&s[i]);
		usleep((random() % 10 + 1) * 10000);
	}

	printf("Car %d crossing finish line!!!\n", d.id);

	free(arg);
	return NULL;
}

int main(int argc, char* argv[]) {

	if(argc != 2) {
		printf("Usage: N\n");
		exit(1);
	}

	int n = atoi(argv[1]);
	int nrt = 1;
	for(int i = 0; i < n; i++) {
		nrt *= 2;
	}

	s = (sem_t*)malloc(sizeof(sem_t) * n);
        srandom(time(NULL));
	pthread_barrier_init(&b, NULL, nrt);

	pthread_t th[nrt];
	for(int i = 0; i < nrt; i++) {
		data* d = (data*)malloc(sizeof(data));
		d->n = n;
		d->id = i;
		if(pthread_create(&th[i], NULL, race, d) != 0) {
			perror("Error creating thread!\n");
			exit(2);
		}
	}
	
	int nrsem = 1;
	for(int i = n - 1; i >= 0; i--) {
                sem_init(&s[i], 0, nrsem);
		nrsem *= 2;	
        }

	for(int i = 0; i < nrt; i++) {
		pthread_join(th[i], NULL);
	}


	for(int i = 0; i < n; i++) {
                sem_destroy(&s[i]);
        }
	pthread_barrier_destroy(&b);
	free(s);
	return 0;
}
