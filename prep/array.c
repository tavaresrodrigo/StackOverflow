#include <stdio.h>

long sum_array(int array[], size_t length) {
    long output = 0;
    
    for (size_t i=0; i <length; i++)
        output += (long)array[i];
    return output;
    
}

int main() {
    int array[] = {1,2,3,4,5,6};
    size_t length = 6;
    printf("The sum of the array elements is %ld\n", sum_array(array, length));
    return 0;
}