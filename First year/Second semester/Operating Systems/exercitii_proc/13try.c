#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// apparently can't fork A too :/

int main(int argc, char** argv) {

	int a2b[2], b2c[2], c2a[2]; //pipes

	if(pipe(a2b) == -1) {
		printf("Error on creating pipe!");
		exit(1);
	}

	if(pipe(b2c) == -1) {
                printf("Error on creating pipe!");
                exit(1);
        }

	if(pipe(c2a) == -1) {
                printf("Error on creating pipe!");
                exit(1);
        }


	int f2 = fork();

	if(f2 == -1) {
		perror("Error creating fork!");
		exit(1);
	}
	else if (f2 == 0) {
		printf("Process B!\n");
		close(b2c[0]);
		close(a2b[1]);
		close(c2a[0]);
		close(c2a[1]);

		int n;

		if(read(a2b[0], &n, sizeof(int)) == -1) {
			perror("Error on read!");
			exit(1);
		}

		int arr[n]; 

		for(int i = 0; i < n; i++) {
			
			if(read(a2b[0], &arr[i], sizeof(int)) == -1) {
				perror("Error on read!");
				exit(1);
			}

			printf("B read all numbers, doing add\n");

			arr[i] += random() % 4 + 2;
		}

		if(write(b2c[1], &n, sizeof(int)) == -1) {
                        perror("Error on read!");
                        exit(1);
                }
		
		for(int i = 0; i < n; i++) {
			if(write(b2c[1], &arr[i], sizeof(int)) == -1) {
				printf("Error on write!");
				exit(1);
                        }
		}

		printf("B wrote\n");
		close(a2b[0]);
		close(b2c[1]);
		exit(0);
	}

	int f3 = fork();

        if(f3 == -1) {
                perror("Error creating fork!");
		exit(1);
        } 
	else if (f3 == 0) {
                printf("Process C!\n");
                close(c2a[0]);
                close(b2c[1]);
		close(a2b[0]);
		close(a2b[1]);

		int n, total = 0;

		if(read(b2c[0], &n, sizeof(int)) == -1) {
                        perror("Error on read!");
                        exit(1);
                }

		int arr[n];

		printf("C read all numbers, doing add\n");

                for(int i = 0; i < n; i++) {
                        if(read(b2c[0], &arr[i], sizeof(int)) == -1) {
                                perror("Error on read!");
				exit(1);
                        }

                        total += arr[i];
                }

		if(write(c2a[1], &total, sizeof(int)) == -1) {
			printf("Error on write!");
			exit(1);
                }

                printf("C wrote %d\n", total);
                close(c2a[1]);
                close(b2c[0]);
		exit(0);
        }

	printf("Process A!\n");
        close(a2b[0]);
        close(c2a[1]);
        close(b2c[0]);
        close(b2c[1]);

	int n, total;

        printf("n = ");
        scanf("%d", &n);

        int arr[n];

        if(write(a2b[1], &n, sizeof(int)) == -1) {
                printf("Error on write!");
                exit(1);
        }

        for (int i = 0; i < n; i++) {
		printf("Give a number: ");
                scanf("%d", &arr[i]);
        }

	for(int i = 0; i < n; i++) {
		if(write(a2b[1], &arr[i], sizeof(int)) == -1) {
			printf("Error on write!");
			exit(1);
		}
	}

	printf("A sent all numbers, going to read\n");

	if(read(c2a[0], &total, sizeof(int)) == -1) {
		perror("Error on read!");
		exit(1);
	}

        printf("A read %d", total);
        close(a2b[1]);
        close(c2a[0]);
	
	wait(0);
	wait(0);
	return 0;
}

