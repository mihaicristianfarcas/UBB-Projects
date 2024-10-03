#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c;
int* s;
int sorted = 0;

void* f(void* arg) {
	int n = *(int*)arg;
	int i, j;

	while(1) {
		i = random() % n;
		j = random() % n;
		pthread_mutex_lock(&m);	

		if(sorted) {
			break;
		}

		int sorted_local = 1;
                for(int i = 0; i < n - 1; i++) {
                        if(s[i] > s[i + 1]) {
                                sorted_local = 0;
                        }
                }

		if(sorted_local) {
                        sorted = sorted_local;
                        break;
                }

		if((i < j && s[i] > s[j]) || (i > j && s[i] < s[j])) {
			int aux = s[i];
			s[i] = s[j];
			s[j] = aux;
			printf("Thread swapped %d with %d\n", s[i], s[j]);
		}

		pthread_mutex_unlock(&m);
	}
	
	pthread_cond_signal(&c);
	pthread_mutex_unlock(&m);
	return NULL;
}

void* fn(void* arg) {
	int n = *((int*)arg);
	pthread_mutex_lock(&m);

	while(!sorted) {
		pthread_cond_wait(&c, &m);
	}

	printf("Array sorted!\n");
	for(int i = 0; i < n; i++) {
                printf("%d ", s[i]);
        }
	printf("\n");
	
	pthread_mutex_unlock(&m);
	return NULL;
}

int main() {

	int n;
	printf("Enter n: ");
	scanf("%d", &n);

	srandom(time(NULL));
	
	s = (int*)malloc(sizeof(int) * n + 1);
	for(int i = 0; i < n; i++) {
		s[i] = random() % 1001;
	}

	pthread_cond_init(&c, NULL);

	printf("The array is: ");
	for(int i = 0; i < n; i++) {
                printf("%d ", s[i]);
        }
	printf("\n");
	
	pthread_t th[n + 1];
	for(int i = 0; i < n; i++) {
		if(pthread_create(&th[i], NULL, f, &n) != 0) {
			perror("Error creating thread!");
			exit(1);
		}
	}

	if(pthread_create(&th[n], NULL, fn, &n) != 0) {
		perror("Error creating thread!");
		exit(1);
	}

	for(int i = 0; i <= n; i++) {
		pthread_join(th[i], NULL);
	}

	pthread_mutex_destroy(&m);
	pthread_cond_destroy(&c);
	free(s);
	return 0;
}
