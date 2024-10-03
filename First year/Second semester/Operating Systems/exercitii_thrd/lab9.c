#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
//Creating a thread for each arg

typedef struct {
        int id;
        char *value;
}pair;

int len = 0;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
sem_t sem;

void* fth(void* a) {
	
	pair* p = (pair*)a;
	printf("Thread %d (%s) started\n", p->id, p->value);
	
	printf("Thread %d before sem_wait\n", p->id);
	sem_wait(&sem);

	printf("Thread %d before lock\n", p->id);
	pthread_mutex_lock(&m);
	
	len += strlen(p->value);

	sleep(rand() % 3 + 3);
	printf("Thread %d before unlock\n", p->id);
	pthread_mutex_unlock(&m);
	
	printf("Thread %d before sem_post\n", p->id);
	sem_post(&sem);
	
	free(p);
	return NULL;
}

int main(int argc, char** argv) {
	if(argc < 2) {
		printf("Add some arguments.");
		exit(1);
	}
	
	int n = argc - 1;
	pthread_t *th = (pthread_t*)malloc(sizeof(pthread_t) * n);
	sem_init(&sem, 0, 3);
	for(int i = 0; i < n; i++) {
		pair *p = (pair*)malloc(sizeof(pair));
		p->id = i;
		p->value = argv[i+1];
		if(pthread_create(&th[i],NULL,fth,p) < 0) {
			perror("Error creating the thread.");
			exit(2);
		}
	}

	for(int i = 0; i < n; i++) {
		pthread_join(th[i], NULL);
	}

	printf("Total size is: %d\n", len);

	pthread_mutex_destroy(&m);
	sem_destroy(&sem);
	free(th);
	return 0;
}	
