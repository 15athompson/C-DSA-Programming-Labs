#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 1000

// Function to initialize memoization array
void initialize_memoization(int memo[], int size) {
    for (int i = 0; i < size; i++) {
        memo[i] = -1; // Initialize all values to -1 to indicate they are not computed
    }
}

// Recursive Fibonacci function with memoization
int fibonacci(int n, int memo[]) {
    if (n <= 1) {
        return n; // Base case: F(0) = 0, F(1) = 1
    }

    // Check if the value has already been computed
    if (memo[n] != -1) {
        return memo[n]; // Return memoized value
    }

    // Compute the value and store it in the memoization array
    memo[n] = fibonacci(n - 1, memo) + fibonacci(n - 2, memo);
    return memo[n];
}

int main() {
    int n;
    printf("Enter an integer n to calculate Fibonacci(n): ");
    scanf("%d", &n);

    if (n < 0) {
        printf("Please enter a non-negative integer.\n");
        return 1;
    }

    int memo[MAX_SIZE]; // Array to hold the memoized values
    initialize_memoization(memo,_SIZE);

    int result = fibonacci(n, memo);
    printf("Fibonacci(%d) = %d\n", n, result);

    return 0;
}
