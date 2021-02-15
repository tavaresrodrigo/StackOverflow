#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char *read_file(char *filename) {
    FILE *f = fopen(filename, "rb"); 
    size_t length; 
    char *output;
    size_t i;
    
    fseek(f, 0, SEEK_END);
    length = ftell(f); 
    fseek(f, 0, SEEK_SET); 
    output = (char *) malloc(length + 1);
    
    for (i = 0; i < length; i++) {
        output[i] = fgetc(f);
    }
    
    output[i] = 0;
    
    fclose(f); 
    
    return output;
}

void func(char *string) {
    char buffer[80];
    int number; 
    getchar();
    strcpy(buffer, string);
}

int main() {
    char *buffer = read_file("hello");
    
    printf("%s\n", buffer);
    func(buffer); 
    
    free(buffer); 
    
    return 0;
}


