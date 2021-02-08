#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc !=2 ) {
        fprintf(stderr, "Pass a number as an argument!\n");
        return 1;
    }    
    int i = 0;
    int n = atoi(argv[1]);
    /* printf("Enter a number:\n");
    scanf("%d", &n);
    */
    printf("Counting to %d with while: \n", n);
    
    while (i <= n) {
        printf ("%d\n", i);
        i++;

    }

    printf("Counting to %d with for: \n", n);
    for (int i =0; i <= n; i++) {
        printf("%d\n", i);
    }
    return 0;   
    
}