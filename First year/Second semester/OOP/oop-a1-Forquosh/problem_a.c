//
// Created by mihai on 05.03.2024.
//

#include <stdio.h>

int is_prime(int n)
// Returns True if n is prime, False otherwise
{
    if(n < 2)
        return 0;
    if(n == 2)
        return 0;
    if(n % 2 == 0)
        return 0;
    for(int i = 3; i * i <= n; i += 2)
        if(n % i == 0)
            return 0;
    return 1;
}

int is_relatively_prime(int a, int b)
// Returns True if a and b are relatively prime, False otherwise
{
    if(a == 1)
        return 0;
    for(int i = 2; i < b; i++)
        if(a % i == 0 && b % i == 0)
            return 0;
    return 1;
}


void determine_numbers()
// Determines and displays the 8 numbers with the required property
{
    printf("The first 8 numbers with the specified property are:\n");
    int cnt = 0;
    for(int n = 3; cnt < 8; n++)
        if (is_prime(n))
        {
            int isPropertySatisfied = 1;
            for (int i = 2; i < n; i++)
                if (!is_relatively_prime(i, n) && !is_prime(i))
                {
                    isPropertySatisfied = 0;
                    break;
                }
            if (isPropertySatisfied) {
                printf("%d ", n);
                cnt++;
            }
        }
}

//int main()
//{
//    determine_numbers();
//    return 0;
//}
