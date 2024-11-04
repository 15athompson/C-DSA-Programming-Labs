#include <stdio.h>
#include <stdlib.h>

#define MAX 1000

int memo[MAX];

int fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    if (memo[n] != -1) {
        return memo[n];
    }
    memo[n] = fibonacci(n - 1) + fibonacci(n - 2);
    return memo[n];
}

int main() {
    int n;
    printf("Enter an integer: ");
    scanf("%d", &n);

    // Initialize memoization array
    for (int i = 0; i < MAX; i++) {
        memo[i] = -1;
    }

    int result = fibonacci(n);
    printf("Fibonacci number for %d is %d\n", n, result);

    return 0;
}
