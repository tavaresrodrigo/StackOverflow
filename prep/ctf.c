#include <stdio.h>
#include <stdlib.h>

float celsius2fahrenheit(float celsius) {
    return (celsius * (9/5)+32);
}
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Pass a temperature in Celsius as an argument!\n");
        return 1;
    }
    float celsius = atof(argv[1]);
    float fahrenheit = celsius2fahrenheit(celsius);
    int fahrenheit_int = (int)fahrenheit;
    printf ("%.2f degrees Celsius is %.2f degrees Fahrenheit.\n", celsius, fahrenheit_int);

    return 0;
}