#include <stdio.h>
int main() {
    int number = 1000000; 
    int *ptr1 = &number; 
    short *ptr2 = (short *)ptr1;    

    printf("We can treat the variable number as a single 32-bit number with a value of %d, or 2 16-bit numbers with values of %hd and %hd\n", *ptr1, *ptr2, *(ptr2 + 1));    
    
    return 0;
}