#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <pthread.h>
#include <semaphore.h>


typedef struct {
	int id;
	int v;
}data;

sem_t sem;
pthread_barrier_t b;


void* fcar(void* a) {
	data* d = (data*) a;
	printf("Car %d started\n", d->id);

	printf("Car %d before barrier\n", d->id);
	pthread_barrier_wait(&b);

	int times = 10;
	while(times>0) {
		printf("Car %d before the bridge\n", d->id);
		sem_wait(&sem);
		
		printf("Car %d crossing the bridge\n", d->id);
		sleep(rand() % 5 + 1);
		
		sem_post(&sem);
		printf("Car %d before the bridge\n", d->id);

		times--;
	}
	free(a);
	return NULL;
}


int main(int argc, char** argv) {
	
	int n, cap, minq;
	printf("Enter the number of cars: ");
	scanf("%d", &n);
	
	printf("Enter the capacity of the bridge: ");
	scanf("%d", &cap);
	sem_init(&sem, 0, cap);

	printf("Enter the minimum queue of the bridge: ");
        scanf("%d", &minq);
	pthread_barrier_init(&b, NULL, minq);
	
	pthread_t* cars = (pthread_t*)malloc(sizeof(pthread_t) * n);

	for(int i = 0; i < n; i++) {
		data *p = (data*)calloc(1, sizeof(data));
		p->id = i;
		p->v = n;
		if(pthread_create(&cars[i], NULL, fcar, p) < 0) {
			printf("Unable to create car!");
			exit(1);
		}
	}

	for(int i = 0; i < n; i++) {
		pthread_join(cars[i], NULL);
	}

	free(cars);
	sem_destroy(&sem);
	pthread_barrier_destroy(&b);
	return 0;
}
