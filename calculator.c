/*
 * CodeAlpha C Programming Internship - Task 1
 * Basic Calculator using switch-case
 * Operations: Addition, Subtraction, Multiplication, Division
 */

#include <stdio.h>

/*Clears leftover characters in the input buffer (e.g. after bad input)*/
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* Reads a number safely; keeps asking until the user types a valid one */
double readNumber(const char *prompt) {
    double value;
    printf("%s", prompt);
    while (scanf("%lf", &value) != 1) {
        printf("  Invalid number. Try again: ");
        clearInputBuffer();
    }
    clearInputBuffer();
    return value;
}

int main(void) {
    char op, again;
    double num1, num2, result;

    printf("=====================================\n");
    printf("        SIMPLE C CALCULATOR\n");
    printf("=====================================\n");

    do {
        num1 = readNumber("\nEnter first number : ");

        printf("Choose operation (+  -  *  /) : ");
        scanf(" %c", &op);
        clearInputBuffer();

        num2 = readNumber("Enter second number: ");

        switch (op) {
            case '+':
                result = num1 + num2;
                printf("\nResult: %.2f + %.2f = %.2f\n", num1, num2, result);
                break;

            case '-':
                result = num1 - num2;
                printf("\nResult: %.2f - %.2f = %.2f\n", num1, num2, result);
                break;

            case '*':
                result = num1 * num2;
                printf("\nResult: %.2f * %.2f = %.2f\n", num1, num2, result);
                break;

            case '/':
                if (num2 == 0) {
                    printf("\nError: Division by zero is not allowed.\n");
                } else {
                    result = num1 / num2;
                    printf("\nResult: %.2f / %.2f = %.2f\n", num1, num2, result);
                }
                break;

            default:
                printf("\nError: '%c' is not a valid operator.\n", op);
        }

        printf("\nCalculate again? (y/n): ");
        scanf(" %c", &again);
        clearInputBuffer();

    } while (again == 'y' || again == 'Y');

    printf("\nThanks for using the calculator. Goodbye!\n");
    return 0;
}