#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>

void* convert(void* arg) {
	char* word = (char*)arg;

	for(int i = 0; i < strlen(word); i++) {
		word[i] = toupper(word[i]);
	}

	return 0;
}

int main(int argc, char* argv[]) {

	if(argc < 2) {
		printf("Please give arguments!");
		exit(1);
	}

	pthread_t threads[argc - 1];
	for(int i = 0; i < argc - 1; i++) {
		pthread_create(&threads[i], NULL, convert, (void*)argv[i + 1]);
	}

	for(int i = 0; i < argc - 1; i++) {
		pthread_join(threads[i], NULL);
	}

	for(int i = 0; i < argc - 1; i++) {
		printf("Word %d: %s\n", i + 1, argv[i + 1]);
	}

	return 0;
}
