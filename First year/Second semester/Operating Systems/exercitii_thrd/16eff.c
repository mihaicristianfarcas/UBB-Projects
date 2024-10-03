#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

// Frequency vector to count digits
int f[10] = {0};

// Mutex for synchronizing access to the frequency vector
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* frequency(void* arg) {
    // Convert argument to an integer
    int n = atoi((char*)arg);

    // Local array to store digit counts for this thread
    int local_f[10] = {0};

    // Count the frequency of digits in the number
    if (n == 0) {
        local_f[0]++;
    } else {
        while (n) {
            local_f[n % 10]++;
            n /= 10;
        }
    }

    // Add local counts to the global frequency vector with synchronization
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < 10; i++) {
        f[i] += local_f[i];
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Not enough arguments!\n");
        exit(1);
    }

    pthread_t threads[argc - 1];

    // Create threads for each argument
    for (int i = 0; i < argc - 1; i++) {
        pthread_create(&threads[i], NULL, frequency, (void*)argv[i + 1]);
    }

    // Wait for all threads to complete
    for (int i = 0; i < argc - 1; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the frequency of each digit
    for (int i = 0; i < 10; i++) {
        printf("Frequency of %d: %d\n", i, f[i]);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}

