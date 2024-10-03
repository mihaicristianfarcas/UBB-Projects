/* 
   Using PIPE channels create and implement the following scenario: 
   Process A reads N integer numbers from the keyboard and sends them another process named B. 
   Process B will add a random number, between 2 and 5, to each received number from process A and will send them to another process named C. 
   Process C will add all the received numbers and will send the result back to process A. All processes will print a debug message before sending and after receiving a number.

*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int main(int argc,char** argv){
    int a2b[2], b2c[2], c2a[2];
    
    pipe(a2b); 
    pipe(b2c); 
    pipe(c2a);
    
    
    int f1 = fork();
    
    if(f1 < 0){
        perror("Error fork1\n");
        exit(1);
    } 
    else if (f1 == 0){
        //B
        //we only read from a->b and write b->c
        close(a2b[1]);
       	close(b2c[0]);
        close(c2a[0]);
       	close(c2a[1]);
       
        srandom(time(NULL));
        
	int n, i;
        int *arr;
        
	//read n from A
        if(0 > read(a2b[0], &n, sizeof(int))){
            close(a2b[0]);
	    close(b2c[1]);
            perror("Error reading n from A\n");
            exit(1);
        }
        
	arr = (int*)malloc(sizeof(int) * n);
        
	for(i = 0; i < n; i++){
            //read from A
            if(0 > read(a2b[0], &arr[i], sizeof(int))){
                close(a2b[0]);
		close(b2c[1]);
                perror("Error reading from A");
                exit(1);
            }

            printf("B got from A: %d\n",arr[i]);

            arr[i] += random()%4 + 2;
        }
        
	//send n to c
        if(0 > write(b2c[1], &n, sizeof(int))){
            close(b2c[1]);
	    close(a2b[0]);
            perror("Error writing n to C\n");
            exit(1);
        }
        
	for(i=0;i<n;i++){
            //send to C
            printf("B sent to C: %d\n",arr[i]);
            
	    if(0>write(b2c[1],&arr[i],sizeof(int))){
                close(b2c[1]);close(a2b[0]);
                perror("Error writing to C\n");
                exit(1);
            }
        }
        
	free(arr);
        
	close(b2c[1]); 
	close(a2b[0]);
        exit(0);
    }
    
    
    int f2=fork();
    
    if(f2<0){
        perror("Error fork2\n");
        exit(1);
    } 
    else if(f2==0){
        //C
        //we only read from b->c and write from c->a
        close(b2c[1]); 
	close(c2a[0]);
        close(a2b[0]); 
	close(a2b[1]);
    
    	//read n from b
        int n, i, *arr;
    	int sum=0;
    
    	if(0 > read(b2c[0], &n, sizeof(int))){
            close(b2c[0]);
	    close(c2a[1]);
            perror("Error reading n from B\n");
            exit(1);
        }
    
    	arr = (int*)malloc(sizeof(int) * n);
        
	for(i = 0; i < n; i++){
            if(0 > read(b2c[0], &arr[i], sizeof(int))){
                close(b2c[0]);
		close(c2a[1]);
                perror("Error reading from B\n");
                exit(1);
            }
            printf("C got from B: %d\n",arr[i]);
        }
        
	for(i=0;i<n;i++){
            sum=sum+arr[i];
        }

        free(arr);
        
	//write c->a
        printf("C sent to A: %d\n",sum);
        
	if(0 > write(c2a[1], &sum, sizeof(int))){
            close(c2a[1]); 
	    close(b2c[0]);
            perror("Error writing from c->a\n");
            exit(1);
        }
        
	close(c2a[1]); 
	close(b2c[0]);
        exit(0);
    }
    
    //A
    //we only write a->b and read from c->a
    close(a2b[0]);
    close(c2a[1]);
    close(b2c[0]);
    close(b2c[1]);
   
    int n, *arr;
    
    //read n from keyboard and send to b
    printf("n = ");
    scanf("%d",&n);
    
    if(0>write(a2b[1],&n,sizeof(int))){
        close(a2b[1]);close(c2a[0]);
        perror("Error sending n to b\n");
        exit(1);
    }
    
    //read n numbers from keyboard and send to b
    int i;
    
    arr = (int*)malloc(sizeof(int) * n);
    
    for(i=0;i<n;i++){
        printf("arr[%d] = ", i);
        scanf("%d",&arr[i]);
    }
    
    for(i=0;i<n;i++){
    
	printf("A sent to B: %d\n",arr[i]);

        if(0>write(a2b[1],&arr[i],sizeof(int))){
            close(a2b[1]);
	    close(c2a[0]);
            perror("Error writing from a->b\n");
            exit(1);
        }
    }
    
    free(arr);
    
    int sum;
    
    //read what c sent back
    if(0>read(c2a[0],&sum,sizeof(int))){
        close(c2a[0]); close(a2b[1]);
        perror("Error reading from C\n");
        exit(1);
    }
    
    printf("A got from C: %d\n", sum);

    close(a2b[1]); close(c2a[0]);
    
    wait(0);
    wait(0);
    return 0;
}
