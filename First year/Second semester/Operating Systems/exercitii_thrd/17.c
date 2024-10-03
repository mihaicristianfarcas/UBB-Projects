#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_LENGTH 100

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int N;
int index = 0;


void *generate_even(void *arg) {
	int* array = (int*)arg;
	pthread_mutex_lock(&mutex);

	while (index < N) {

		while (index % 2 != 0 && index < N) {
			pthread_cond_wait(&cond, &mutex);
		}

		if (index >= N) {
			break;
		}

		array[index++] = random() % 51 * 2;

		pthread_cond_signal(&cond);
	}

	pthread_mutex_unlock(&mutex);

	return NULL;
}


void *generate_odd(void *arg) {
	int* array = (int*)arg;
	pthread_mutex_lock(&mutex);

	while (index < N) {

		while (index % 2 == 0 && index < N) {
			pthread_cond_wait(&cond, &mutex);
		}

		if (index >= N) {
			break;
		}

		array[index++] = random() % 51 * 2 - 1;
		pthread_cond_signal(&cond);
	}

	pthread_mutex_unlock(&mutex);

	return NULL;
}

int main() {

	printf("Enter the value of N: ");
	scanf("%d", &N);
	int array[N];
	srandom(time(0));


	pthread_t thread_even, thread_odd;
	pthread_create(&thread_even, NULL, generate_even, (void*)array);
	pthread_create(&thread_odd, NULL, generate_odd, (void*)array);

	pthread_join(thread_even, NULL);
	pthread_join(thread_odd, NULL);

	printf("Generated array: ");
	for (int i = 0; i < N; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");

	return 0;
}

