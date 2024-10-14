#include <stdio.h>
#include <string.h>

#define MAX_NAMES 8
#define MAX_LENGTH 20

void bubbleSort(char names[MAX_NAMES][MAX_LENGTH], int n) {
    int i, j;
    char temp[MAX_LENGTH];
    
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (strcmp(names[j], names[j + 1]) < 0) {
                strcpy(temp, names[j]);
                strcpy(names[j], names[j + 1]);
                strcpy(names[j + 1], temp);
            }
        }
    }
}

void insertionSort(char names[MAX_NAMES][MAX_LENGTH], int n) {
    int i, j;
    char key[MAX_LENGTH];
    
    for (i = 1; i < n; i++) {
        strcpy(key, names[i]);
        j = i - 1;
        
        while (j >= 0 && strcmp(names[j], key) < 0) {
            strcpy(names[j + 1], names[j]);
            j = j - 1;
        }
        strcpy(names[j + 1], key);
    }
}

void printNames(char names[MAX_NAMES][MAX_LENGTH], int n) {
    for (int i = 0; i < n; i++) {
        printf("%s\n", names[i]);
    }
    printf("\n");
}

int main() {
    char names[MAX_NAMES][MAX_LENGTH] = {"Matthew", "Andrew", "Dylan", "Melody", "Nancy", "Imran", "Jacob", "Juliet"};
    int n = MAX_NAMES;
    
    printf("Original array:\n");
    printNames(names, n);
    
    bubbleSort(names, n);
    printf("Sorted array (Bubble Sort):\n");
    printNames(names, n);
    
    char names2[MAX_NAMES][MAX_LENGTH] = {"Matthew", "Andrew", "Dylan", "Melody", "Nancy", "Imran", "Jacob", "Juliet"};
    
    insertionSort(names2, n);
    printf("Sorted array (Insertion Sort):\n");
    printNames(names2, n);
    
    return 0;
}
