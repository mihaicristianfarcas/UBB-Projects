#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#define size 10

typedef struct {
	int id;
	char* buf;
	int* pos;
	int* repeats;
	int maxSize;
	pthread_mutex_t *m;
	pthread_cond_t *c;
}data;

void* gen(void* arg) {
	data d = *((data*)arg);

	while(1) {

		pthread_mutex_lock(d.m);
		if(*(d.repeats) == 0) {
			pthread_cond_broadcast(d.c);
			pthread_mutex_unlock(d.m);
			break;
		}
		while(*(d.pos) == d.maxSize) {
			pthread_cond_broadcast(d.c);
			pthread_cond_wait(d.c, d.m);
		}

		char c = rand() % 26 + 'a';
		d.buf[*(d.pos)] = c;
		*(d.pos) += 1;
		pthread_mutex_unlock(d.m);
	}

	return NULL;
}

void* printer(void* arg) {
	data d = *((data*)arg);

	while(1) {

		pthread_mutex_lock(d.m);
		if(*(d.repeats) == 0) {
			pthread_cond_broadcast(d.c);
			pthread_mutex_unlock(d.m);
			break;
		}

		while(*(d.pos) < d.maxSize) {
			pthread_cond_broadcast(d.c);
			pthread_cond_wait(d.c, d.m);
		}

		printf("%s\n", d.buf);
		memset(d.buf, 0, sizeof(char) * d.maxSize);
		*(d.pos) = 0;
		*(d.repeats) -= 1;
		pthread_mutex_unlock(d.m);
	}

	return NULL;
}

int main(int argc, char* argv[]) {
	
	if(argc != 3) {
		printf("Usage: N, M\n");
		exit(1);
	}	

	int n = atoi(argv[1]);
	int m = atoi(argv[2]);	
	int pos = 0;

	srand(time(NULL));

	pthread_mutex_t mtx;
	pthread_cond_t cond;
	pthread_mutex_init(&mtx, NULL);
	pthread_cond_init(&cond, NULL);
	
	pthread_t th[n + 1];
	data args[n + 1];
	char* buf = (char*)malloc(sizeof(char) * size);

	for(int i = 0; i < n; i++) {
		args[i].id = i;
		args[i].m = &mtx;
		args[i].c = &cond;
		args[i].repeats = &m;
		args[i].pos = &pos;
		args[i].buf = buf;
		args[i].maxSize = size;

		if(pthread_create(&th[i], NULL, gen, &args[i]) != 0) {
			perror("Error creating thread\n");
			exit(2);
		}
	}

	args[n].id = n;
	args[n].m = &mtx;
	args[n].c = &cond;
	args[n].repeats = &m;
	args[n].pos = &pos;
	args[n].buf = buf;
	args[n].maxSize = size;
	if(pthread_create(&th[n], NULL, printer, &args[n]) != 0) {
		perror("Error creating thread\n");
		exit(2);
	}
	
	for(int i = 0; i <= n; i++) {
                pthread_join(th[i], NULL);
        }

        free(buf);
        pthread_mutex_destroy(&mtx);
        pthread_cond_destroy(&cond);
	return 0;
}
