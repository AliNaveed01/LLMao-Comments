#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int factorial(int n) {
    if (n <= 1) return 1;
    else return n * factorial(n-1);
}

void say_hello() {
    printf("Hello, World!\n");
}

int main() {
    printf("Add: %d\n", add(3, 5));
    printf("Factorial: %d\n", factorial(5));
    say_hello();
    return 0;
}
