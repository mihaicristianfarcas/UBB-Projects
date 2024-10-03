//
// Created by mihai on 05.03.2024.
//

#include <stdio.h>

int same_digits(int a, int b)
{
    int v1[10] = {0}, v2[10] = {0};
    while(a != 0)
    {
        v1[a % 10] = 1;
        a /= 10;
    }
    while(b != 0)
    {
        v2[b % 10] = 1;
        b /= 10;
    }
    for(int i = 0; i < 10; i++)
        if(v1[i] != v2[i])
            return 0;
    return 1;
}

void longest_continuous_subsequence(int a[], int len) {
    int max_length = 1;
    int start_index = 0;
    int current_length = 1;
    int current_start = 0;

    for (int i = 1; i < len; i++) {
        if (same_digits(a[i - 1], a[i])) {
            current_length++;
            if (current_length > max_length) {
                max_length = current_length;
                start_index = current_start;
            }
        }
        else {
            current_length = 1;
            current_start = i;
        }
    }

    printf("Longest contiguous subsequence with same digits:\n");
    for (int i = start_index; i < start_index + max_length; i++)
        printf("%d ", a[i]);
}



int main(){
    int a[] = {125, 321, 231, 122, 789, 987, 879, 798, 333};
    int len = sizeof(a) / sizeof(a[0]);

    longest_continuous_subsequence(a, len);
    return 0;
}