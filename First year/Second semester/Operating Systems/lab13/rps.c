#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct {
	int id;
	int lives;
	int wins;
	int lastWon;
	int n;
}data;

// 0 = rock
// 1 = paper
// 2 = scissors

int *moves;
pthread_barrier_t *b;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
int doneGame = 0;
int lostPairs = 0;

void* player(void* a) {
	
	data *d = (data*)a;
	int id = d->id;
	int n = d->n;
	
	int localDoneGame = 0;
        int localPairs = 0;

	int otherPlayerId = id % 2 == 0 ? id + 1 : id - 1;
	printf("Player %d started. Other player is %d\n", id, otherPlayerId);

	// Segmentation fault??

	while(d->lives > 0 || localDoneGame == 1 || localPairs < n) {
		
		int move = rand() % 3;
        	moves[id] = move;
        	printf("Player %d's move: %d\n", id, move);
		
		pthread_barrier_wait(&b[id / 2]);
		printf("Player %d checking...\n", id);

		if(moves[id] == moves[otherPlayerId]) {
			printf("%d vs %d : DRAW\n", id, otherPlayerId);

		} else if(
				(moves[id] == 0 && moves[otherPlayerId] == 2) ||
				(moves[id] == 1 && moves[otherPlayerId] == 0) || 
				(moves[id] == 2 && moves[otherPlayerId] == 1)) 
		{
			printf("Player %d won\n", id);
			d->wins++;
			if(d->lastWon == 1) {
				d->wins++;
			}
			d->lastWon = 1;
		
		} else {
			printf("Player %d lost\n", id);
			d->lives--;
			d->lastWon = 0;
		}

		pthread_mutex_lock(&m);
		
		if(d->wins == 5) {
			doneGame = 1;
			printf("Player %d won the games!\n", id);
		}
		
		localDoneGame = doneGame;
		
		if(d->lives == 0) {
			lostPairs++;
		}
		
		localPairs--;

		pthread_mutex_unlock(&m);
		
		if(id % 2 == 0) {
			printf("Player %d resetting the barrier\n", id);
			pthread_barrier_init(&b[id / 2], NULL, 2);
		}
	}

	free(d);
	return NULL;
}

int main(int argc, char** argv) {

	if(argc != 2) {
		printf("Usage: N\n");
		exit(1);
	}

	int n = atoi(argv[1]);
	printf("Nr. pairs is %d\n", n);
	int* moves = (int*)malloc(sizeof(int) * n * 2);
	
	b = (pthread_barrier_t*)malloc(sizeof(pthread_barrier_t) * n);
	for(int i = 0; i < n; i++) {
		pthread_barrier_init(&b[i], NULL, 2);
	}

	pthread_t *th = (pthread_t*)malloc(sizeof(pthread_t) * n * 2);
	for(int i = 0; i < n * 2; i++) {
	
		moves[i] = -1;

		data *d = (data*)malloc(sizeof(data));
		d->id = i;
		d->lives = 3;
		d->wins = 0;
		d->lastWon = 0;
		d->n = n;
		
		if(pthread_create(&th[i], NULL, player, d) < 0) {
			perror("Unable to create thread\n");
			exit(2);
		}
	}

	free(th);
	free(moves);
	return 0;
}
