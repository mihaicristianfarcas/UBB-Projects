#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_ROWS 100
#define MAX_COLS 100

int matrix[MAX_ROWS][MAX_COLS];
int sums[MAX_ROWS];
int rows, cols, total = 0;

pthread_mutex_t m;


void *sum_row(void *arg) {
    int row = *((int *)arg);
    free(arg);

    sums[row] = 0;
    for (int j = 0; j < cols; j++) {
        sums[row] += matrix[row][j];
    }

    //pthread_mutex_lock(&m);
    total += sums[row];
    //pthread_mutex_unlock(&m);

    return NULL;
}

int main() {
    FILE *file = fopen("matrix.txt", "r");
    if (file == NULL) {
        perror("Unable to open file");
        return 1;
    }

    // Read the matrix dimensions
    fscanf(file, "%d %d", &rows, &cols);

    // Read the matrix elements
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fscanf(file, "%d", &matrix[i][j]);
        }
    }

    fclose(file);

    pthread_t threads[MAX_ROWS];
    pthread_mutex_init(&m, NULL);

    // Create threads to sum each row
    for (int i = 0; i < rows; i++) {
        int *row = malloc(sizeof(int));
        *row = i;
        pthread_create(&threads[i], NULL, sum_row, (void *)row);
    }

    // Wait for all threads to complete
    for (int i = 0; i < rows; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the results
    for (int i = 0; i < rows; i++) {
        printf("Sum of row %d: %d\n", i, sums[i]);
    }

    printf("Total is: %d\n", total);
	
    return 0;
}

