#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

void* oddf(void* arg) {
	int* arr = (int*)arg;
	int nr = random() % 100;
	nr += nr % 2 + 1;
	return 0;
}

void* evenf(void* arg) {
	int* arr = (int*)arg;
	int nr = random() % 100;
	nr += nr % 2;
	return 0;
}

int main() {
	
	int n;
	printf("Please give N: ");
	scanf("%d", &n);
	int arr[n];

	srandom(time(0));
	pthread_t odd, even;
	
	pthread_create(&odd, NULL, oddf, (void*)arr);
	pthread_create(&even, NULL, evenf, (void*)arr);

	pthread_join(odd, NULL);
	pthread_join(even, NULL);


	return 0;
}
