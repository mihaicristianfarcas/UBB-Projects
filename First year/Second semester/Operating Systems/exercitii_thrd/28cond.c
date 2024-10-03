#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    int *arr, n, *sorted;
    pthread_mutex_t *m;
    pthread_cond_t *c;
} data;

void *sorter(void *arg) {
    data *d = (data*) arg;
    int i, j, k;
    while(1) {
        i = random() % d->n;
        j = random() % d->n;
        int sorted = 1;
        pthread_mutex_lock(d->m);
        if(*d->sorted)
            break;
        for(k = 0; k < d->n - 1; k++) {
            if(d->arr[k] > d->arr[k+1])
                sorted = 0;
        }
        if(sorted) {
            *d->sorted = sorted;
            break;
        }
        if((i < j && d->arr[i] > d->arr[j]) ||
                (i > j && d->arr[i] < d->arr[j])) {
            int aux = d->arr[i];
            d->arr[i] = d->arr[j];
            d->arr[j] = aux;
        }
        pthread_mutex_unlock(d->m);
        usleep(10);
    }
    pthread_cond_signal(d->c);
    pthread_mutex_unlock(d->m);
    return NULL;
}

void *printer(void *arg) {
    data *d = (data*) arg;
    pthread_mutex_lock(d->m);
    while(*d->sorted == 0) pthread_cond_wait(d->c, d->m);
    int i;
    for(i = 0; i < d->n; i++) {
        printf("%5d", d->arr[i]);
    }
    printf("\n");
    pthread_mutex_unlock(d->m);
    return NULL;
}

int main(int argc, char *argv[]) {
    int n;
    printf("n = ");
    scanf("%d", &n);
    srandom(time(NULL));
    int i;
    int *arr = malloc(sizeof(int) * n);
    int *sorted = malloc(sizeof(int));
    *sorted = 0;
    data *args = malloc(sizeof(data) * (n + 1));
    pthread_mutex_t *m = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(m, NULL);
    pthread_cond_t *c = malloc(sizeof(pthread_cond_t));
    pthread_cond_init(c, NULL);
    for(i = 0; i < n; i++) {
        arr[i] = random() % 1001;
        args[i].m = m;
        args[i].c = c;
        args[i].arr = arr;
        args[i].n = n;
        args[i].sorted = sorted;
        printf("%5d", arr[i]);
    }
    printf("\n");
    args[n].m = m;
    args[i].c = c;
    args[n].arr = arr;
    args[n].n = n;
    args[n].sorted = sorted;

    pthread_t *th = malloc(sizeof(pthread_t) * (n + 1));
    for(i = 0; i < n; i++) {
        if(0 > pthread_create(&th[i], NULL, sorter, &args[i])) {
            perror("Error on thread create");
        }
    }
    if(0 > pthread_create(&th[n], NULL, printer, &args[n])) {
        perror("Error on thread create");
    }
    for(i = 0; i < n + 1; i++) {
        pthread_join(th[i], NULL);
    }

    pthread_mutex_destroy(m);
    pthread_cond_destroy(c);
    free(th);
    free(args);
    free(arr);
    free(sorted);
    free(m);
    free(c);
    return 0;
}
