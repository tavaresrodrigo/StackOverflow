#include <stdio.h>
int main() {
        char string[] = { 'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o','r', 'l', 'd', '!', '\n', 0 }; 
        char rodrigo[] = {'R','o','d','r','i','g','o','\n',0};
        char *another_string = "This is an example!\n";  
        printf(string);
        printf(rodrigo);    
        printf(another_string);     
        return 0;
}