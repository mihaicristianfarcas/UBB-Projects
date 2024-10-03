#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/wait.h>
#include <string.h>

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;

int sum = 0;
char *vowels;

void* func1(void* arg) {
	char* word = (char*)arg;

	for(int i = 0; i < strlen(word); i++) {
		if(strchr("aeiouAEIOU", word[i]) != NULL) {
			pthread_mutex_lock(&m);
			int len = strlen(vowels);
			vowels[len] = word[i];
			vowels[len + 1] = '\0';
			pthread_mutex_unlock(&m);
		}
	}

	return NULL;
}

void* func2(void* arg) {
	char* word = (char*)arg;

	for(int i = 0; i < strlen(word); i++) {
		if(word[i] >= '0' && word[i] <= '9') {
			pthread_mutex_lock(&m2);
			sum += (word[i] - '0');
			pthread_mutex_unlock(&m2);
		}
	}

	return NULL;
}

int main(int argc, char* argv[]) {
	
	if(argc == 1) {
		printf("Usage: strings\n");
		exit(1);
	}

	int c12p[2], c22p[2];
	pipe(c12p); pipe(c22p);

	vowels = (char*)malloc(sizeof(char) * 1000);
	int f1 = fork();
	if(f1 < 0) {
		perror("MY CHILD!...\n");
		exit(2);
	} else if(f1 == 0) {
		close(c12p[0]);
		close(c22p[0]);
		close(c22p[1]);

		pthread_t th[argc - 1];
		for(int i = 0; i < argc - 1; i++) {
			if(pthread_create(&th[i], NULL, func1, argv[i + 1]) != 0) {
				perror("Error creating thread\n");
				exit(2);
			}
		}

		for(int i = 0; i < argc - 1; i++) {
			pthread_join(th[i], NULL);
		}

		printf("Vowels: %s\n", vowels);
	
		close(c12p[1]);
		exit(0);
	}

	int f2 = fork();
	if(f2 < 0) {
		perror("MY CHILD!...\n");                                                   exit(2);
	} else if(f2 == 0) {
		close(c22p[0]);
		close(c12p[0]);
		close(c12p[1]);

		pthread_t th[argc - 1];
		for(int i = 0; i < argc - 1; i++) {
			if(pthread_create(&th[i], NULL, func2, argv[i + 1]) != 0) {
				perror("Error creating thread\n");
				exit(2);
			}
		}

		for(int i = 0; i < argc - 1; i++) {
			pthread_join(th[i], NULL);
		}

		printf("Total: %d\n", sum);

		close(c22p[1]);
		exit(0);
	}
	
	
	wait(0);
	wait(0);
	free(vowels);
	return 0;
}
