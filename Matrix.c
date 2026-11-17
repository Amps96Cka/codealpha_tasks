/*
 * CodeAlpha C Programming Internship - Task 2
 * Matrix Operations: Addition, Multiplication, Transpose
 * Uses functions and 2D arrays.
 */

#include <stdio.h>

#define MAX 10

void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* Reads an integer within [min, max]; keeps asking until valid */
int readInt(const char *prompt, int min, int max) {
    int value;
    printf("%s", prompt);
    while (scanf("%d", &value) != 1 || value < min || value > max) {
        printf("  Please enter a whole number between %d and %d: ", min, max);
        clearInputBuffer();
    }
    clearInputBuffer();
    return value;
}

void readMatrix(int m[MAX][MAX], int rows, int cols, const char *name) {
    printf("\nEnter elements of matrix %s (%d x %d):\n", name, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("  %s[%d][%d] = ", name, i + 1, j + 1);
            while (scanf("%d", &m[i][j]) != 1) {
                printf("  Invalid. Enter an integer: ");
                clearInputBuffer();
            }
        }
    }
    clearInputBuffer();
}

void printMatrix(int m[MAX][MAX], int rows, int cols, const char *title) {
    printf("\n%s (%d x %d):\n", title, rows, cols);
    for (int i = 0; i < rows; i++) {
        printf("  ");
        for (int j = 0; j < cols; j++) {
            printf("%5d ", m[i][j]);
        }
        printf("\n");
    }
}

/* C = A + B (same dimensions required) */
void addMatrices(int a[MAX][MAX], int b[MAX][MAX], int c[MAX][MAX],
                 int rows, int cols) {
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            c[i][j] = a[i][j] + b[i][j];
}

/* C = A x B  (A is r1 x c1, B is c1 x c2, result is r1 x c2) */
void multiplyMatrices(int a[MAX][MAX], int b[MAX][MAX], int c[MAX][MAX],
                      int r1, int c1, int c2) {
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            c[i][j] = 0;
            for (int k = 0; k < c1; k++)
                c[i][j] += a[i][k] * b[k][j];
        }
    }
}

/* T = transpose of A (A is rows x cols, T is cols x rows) */
void transposeMatrix(int a[MAX][MAX], int t[MAX][MAX], int rows, int cols) {
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            t[j][i] = a[i][j];
}

void doAddition(void) {
    int a[MAX][MAX], b[MAX][MAX], c[MAX][MAX];
    int rows = readInt("\nNumber of rows (1-10): ", 1, MAX);
    int cols = readInt("Number of columns (1-10): ", 1, MAX);

    readMatrix(a, rows, cols, "A");
    readMatrix(b, rows, cols, "B");

    addMatrices(a, b, c, rows, cols);
    printMatrix(a, rows, cols, "Matrix A");
    printMatrix(b, rows, cols, "Matrix B");
    printMatrix(c, rows, cols, "A + B");
}

void doMultiplication(void) {
    int a[MAX][MAX], b[MAX][MAX], c[MAX][MAX];
    int r1 = readInt("\nRows of A (1-10): ", 1, MAX);
    int c1 = readInt("Columns of A (1-10): ", 1, MAX);
    printf("Matrix B must have %d rows to multiply with A.\n", c1);
    int c2 = readInt("Columns of B (1-10): ", 1, MAX);

    readMatrix(a, r1, c1, "A");
    readMatrix(b, c1, c2, "B");

    multiplyMatrices(a, b, c, r1, c1, c2);
    printMatrix(a, r1, c1, "Matrix A");
    printMatrix(b, c1, c2, "Matrix B");
    printMatrix(c, r1, c2, "A x B");
}

void doTranspose(void) {
    int a[MAX][MAX], t[MAX][MAX];
    int rows = readInt("\nNumber of rows (1-10): ", 1, MAX);
    int cols = readInt("Number of columns (1-10): ", 1, MAX);

    readMatrix(a, rows, cols, "A");

    transposeMatrix(a, t, rows, cols);
    printMatrix(a, rows, cols, "Matrix A");
    printMatrix(t, cols, rows, "Transpose of A");
}

int main(void) {
    int choice;

    do {
        printf("\n=====================================\n");
        printf("         MATRIX OPERATIONS\n");
        printf("=====================================\n");
        printf("  1. Matrix Addition\n");
        printf("  2. Matrix Multiplication\n");
        printf("  3. Transpose\n");
        printf("  4. Exit\n");
        choice = readInt("Choose an option (1-4): ", 1, 4);

        switch (choice) {
            case 1: doAddition();       break;
            case 2: doMultiplication(); break;
            case 3: doTranspose();      break;
            case 4: printf("\nGoodbye!\n"); break;
        }
    } while (choice != 4);

    return 0;
}