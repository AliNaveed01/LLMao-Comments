// This function takes two integer parameters and returns their sum.
include <stdio.h>

int add(int a, int b) {
    return a + b;
}

// This function calculates the factorial of a given integer 'n' recursively.


int factorial(int n) {
    if (n <= 1) return 1;
    else return n * factorial(n-1);
}

// This function prints the message "Hello, World!" to the console when called.


void say_hello() {
    printf("Hello, World!\n");
}

/* 
This main function demonstrates how to use three different functions: add, factorial, and say_hello. 
It prints the sum of two numbers, the factorial of a number, and outputs a greeting message.
*/ 


int main() {
    printf("Add: %d\n", add(3, 5));
    printf("Factorial: %d\n", factorial(5));
    say_hello();
    return 0;
}

