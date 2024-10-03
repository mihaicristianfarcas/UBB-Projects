#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <pthread.h>
#include <semaphore.h>

sem_t barber_ready;
sem_t client_ready;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

int free_chairs = 4;

void* fbarber(void* a) {

	printf("Barber is working\n");
	
	while(1) {
		printf("Barber is sleeping\n");
		sem_wait(&client_ready);
	
		pthread_mutex_lock(&m);
		free_chairs++;
		pthread_mutex_unlock(&m);
	
		sem_post(&barber_ready);
		printf("Barber is serving a client\n");
		sleep(rand() % 10 + 1);
	}
	return NULL;
}

void* fclient(void* a) {

	int id = *(int*)a;
	printf("Client %d started\n", id);
	sleep(rand() % 3 + 1);
	
	pthread_mutex_lock(&m);
	if(free_chairs > 0) {
		printf("Client %d entered\n", id);
		free_chairs--;
		pthread_mutex_unlock(&m);
		
		sem_post(&client_ready);
		printf("Client %d waiting for the barber\n", id);
		
		sem_wait(&barber_ready);
		printf("Client %d is done\n", id);
	} else {
		printf("Client %d left\n", id);
		pthread_mutex_unlock(&m);
	}

	free(a);
        return NULL;
}

int main(int argc, char** argv) {
	
	int n;
	pthread_t b;
	pthread_t* c;
	
	if(argc > 1) {
		n = atoi(argv[1]);
		printf("Number of clients in %d\n", n);
		c = (pthread_t*)malloc(sizeof(pthread_t) * n);
		if(pthread_create(&b, NULL, fbarber, NULL) < 0) {
			printf("Unable to create the barber\n");
			exit(1);
		}

		for(int i = 0; i < n; i++) {
			int *p = (int*)malloc(sizeof(int));
			*p = i;
			if(pthread_create(&c[i], NULL, fclient, p) < 0) {
				printf("Unable to create the client\n");
				exit(2);
			}
		}	

		pthread_join(b, NULL);
		for(int i = 0; i < n; i++) {
			pthread_join(c[i], NULL);
		}

		free(c);

	} else {
		printf("Usage N number of clients\n");
	}
	
	return 0;
}
