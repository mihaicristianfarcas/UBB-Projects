/*
 * Write a C program that creates two child processes. The two child processes will 
 * alternate sending random integers between 1 and 10(inclusively) to one another 
 * until one of them sends the number 10. Print messages as the numbers are sent.
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    int c1_to_c2[2], c2_to_c1[2];
    
    if(-1 == pipe(c1_to_c2)) {
        perror("Error on first pipe");
        exit(1);
    }
    
    if(-1 == pipe(c2_to_c1)) {
        perror("Error on second pipe");
        exit(1);
    }

    int f = fork();
    
    if(-1 == f) {
        perror("Error on fork");
    } 
    
    else if (0 == f) {
    
        close(c2_to_c1[1]);
        close(c1_to_c2[0]);
    
    	int n;
    	srandom(getpid());
    
    	if(0 > read(c2_to_c1[0], &n, sizeof(int))) {
            perror("Child 1: Error on read number from pipe");
        }
    
    	printf("Child 1 read %d\n", n);
    
    	while(n != 10) {
            n = random() % 10 + 1;
            if(0 > write(c1_to_c2[1], &n, sizeof(int))) {
                perror("Child 1: Error on write in pipe");
            }
    
    	    if(n == 10) {
                break;
            }
    
    	    if(0 > read(c2_to_c1[0], &n, sizeof(int))) {
                perror("Child 2: Error on read number from pipe");
            }
            printf("Child 1 read %d\n", n);
        }
    
    	close(c2_to_c1[0]);
        close(c1_to_c2[1]);
        exit(0);
    }

    f = fork();
    
    if(-1 == f) {
        perror("Error on fork");
    } 
    else if (f == 0) {
    
	close(c2_to_c1[0]);
        close(c1_to_c2[1]);
    
    	int n = 0;
        srandom(getpid());
    
    	while(n != 10) {
            n = random() % 10 + 1;
    
    	    if(0 > write(c2_to_c1[1], &n, sizeof(int))) {
                perror("Child 2: Error on write in pipe");
            }
    
    	    if(n == 10) {
                break;
            }
    
    	    if(0 > read(c1_to_c2[0], &n, sizeof(int))) {
                perror("Child 2: Error on read from pipe");
            }
    
    	    printf("Child 2 read %d\n", n);
        }
    
    	close(c2_to_c1[1]);
        close(c1_to_c2[0]);
        exit(0);
    }
    
    wait(0);
    wait(0);
    return 0;
}
