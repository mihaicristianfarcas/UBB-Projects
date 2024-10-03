#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int f[10];
pthread_mutex_t m;

void* frequency(void* arg) {
	//int n = *(int*)arg;
	int n = atoi((char*)arg);

	// this is sequential, not efficient
	pthread_mutex_lock(&m);
	if(n == 0) {
		f[0]++;
	} else {
		while(n) {
			f[n%10]++;
			n /= 10;
		}
	}
	pthread_mutex_unlock(&m);

	return NULL;
}

int main(int argc, char** argv) {
	
	if(argc < 2) {
		printf("Not enough arguments!");
		exit(1);
	}

	pthread_t threads[argc - 1];

	for(int i = 0; i < argc - 1; i++) {
		pthread_create(&threads[i], NULL, frequency, (void*)argv[i + 1] );
	}

	pthread_mutex_init(&m, NULL);

	for(int i = 0; i < argc - 1; i++) {
		pthread_join(threads[i], NULL);
	}

	for(int i = 0; i < 10; i++) {
		printf("Frequency of %d: %d\n", i, f[i]);
	}

	return 0;
}
