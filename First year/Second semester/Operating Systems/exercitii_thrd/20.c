#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

int n, m;

pthread_barrier_t b;
sem_t s;

void* race(void* arg) {
	int car_number = *(int*)arg;
	free(arg);

	printf("Car %d before start!\n", car_number);
	pthread_barrier_wait(&b);
	
	for(int i = 0; i < m; i++) {
		sem_wait(&s);
		printf("Car %d crossing checkpoint %d!\n", car_number, i);
		sem_post(&s);
		usleep((random() % 10 + 1) * 10000);
	}

	printf("Car %d crossing finish line!\n", car_number);
	return NULL;
}

int main(int argc, char* argv[]) {

	if(argc != 3) {
		printf("Please provide exactly two arguments (Nr. cars and checkpoints)!\n");
		exit(1);
	}

        n = atoi(argv[1]);
	m = atoi(argv[2]);
	pthread_t racers[n];
	pthread_barrier_init(&b, NULL, n);
	sem_init(&s, 0, 1);

	srandom(time(NULL));

	for(int i = 1; i <= n; i++) {
		int* car_number = (int*)malloc(sizeof(int));
		*car_number = i;
		pthread_create(&racers[i], NULL, race, (void*)car_number);
	}

	for(int i = 1; i <= n; i++) {
		pthread_join(racers[i], NULL);
	}

	pthread_barrier_destroy(&b);
	sem_destroy(&s);
	return 0;
}
