#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int a2d[2], b2d[2], c2d[2], d2b[2], d2c[2];
    if (0 > pipe(a2d)) {
        perror("Error on pipe 1: ");
        exit(1);
    }
    if (0 > pipe(b2d)) {
        perror("Error on pipe 2: ");
        exit(1);
    }
    if (0 > pipe(c2d)) {
        perror("Error on pipe 3: ");
        exit(1);
    }
    if (0 > pipe(d2b)) {
        perror("Error on pipe 4: ");
        exit(1);
    }
    if (0 > pipe(d2c)) {
        perror("Error on pipe 5: ");
        exit(1);
    }
    int fb = fork();
    if (0 > fb) {
        perror("Error on fork 1: ");
        exit(1);
    } else if (0 == fb) {
        close(a2d[0]);
        close(b2d[0]);
        close(c2d[0]);
        close(d2c[0]);
        close(a2d[1]);
        close(c2d[1]);
        close(d2b[1]);
        close(d2c[1]);
        srandom(getpid());
        int b;
        while(1) {
            b = random() % 200 + 1;
            printf("Process B: %d\n", b);
            if (0 > write(b2d[1], &b, sizeof(int))) {
                perror("Error on write B to D");
                exit(1);
            }
            if (0 > read(d2b[0], &b, sizeof(int))) {
                perror("Error read B from D");
                exit(1);
            }
            if(b == -1) break;
        }
        close(b2d[1]);
        close(d2b[0]);
        exit(0);
    }
    
    int fc = fork();
    if (0 > fc) {
        perror("Error on fork 2: ");
        exit(1);
    } else if (0 == fc) {
        close(a2d[0]);
        close(b2d[0]);
        close(c2d[0]);
        close(d2b[0]);
        close(a2d[1]);
        close(b2d[1]);
        close(d2b[1]);
        close(d2c[1]);
        srandom(getpid());
        int c;
        while(1) {
            c = random() % 200 + 1;
            printf("Process C: %d\n", c);
            if (0 > write(c2d[1], &c, sizeof(int))) {
                perror("Error on write C to D");
                exit(1);
            }
            if (0 > read(d2c[0], &c, sizeof(int))) {
                perror("Error read C from D");
                exit(1);
            }
            if(c == -1) break;
        }
        close(c2d[1]);
        close(d2c[0]);
        exit(0);
    }

    int fd = fork();
    if (0 > fd) {
        perror("Error on fork 3: ");
        exit(1);
    } else if (0 == fd) {
        close(d2b[0]);
        close(d2c[0]);
        close(a2d[1]);
        close(b2d[1]);
        close(c2d[1]);
        int nr;
        if (0 > read(a2d[0], &nr, sizeof(int))) {
            perror("Error read D from A");
            exit(1);
        }

        int b, c, stop = 0;
        while (stop == 0) {
            if (0 > read(b2d[0], &b, sizeof(int))) {
                perror("Error read D from B");
                exit(1);
            }
            if (0 > read(c2d[0], &c, sizeof(int))) {
                perror("Error read D from C");
                exit(1);
            }
            int dif = abs(b - c);
            printf("Process D: %d\n", dif);
            if (dif <= nr) 
                stop = -1;
            if (0 > write(d2b[1], &stop, sizeof(int))) {
                perror("Error on write D to B");
                exit(1);
            }
            if (0 > write(d2c[1], &stop, sizeof(int))) {
                perror("Error on write D to C");
                exit(1);
            }
        }
        close(a2d[0]);
        close(b2d[0]);
        close(c2d[0]);
        close(d2b[1]);
        close(d2c[1]);
        exit(0);
    }

    close(a2d[0]);
    close(b2d[0]);
    close(c2d[0]);
    close(d2b[0]);
    close(d2c[0]);
    close(b2d[1]);
    close(c2d[1]);
    close(d2b[1]);
    close(d2c[1]);

    srandom(getpid());
    int nr = random() % 11 + 10;
    printf("Process A: %d\n", nr);
    if (0 > write(a2d[1], &nr, sizeof(int))) {
        perror("Error on write A to D");
        exit(1);
    }

    wait(0);
    wait(0);
    wait(0);
    close(a2d[1]);
    return 0;
}
