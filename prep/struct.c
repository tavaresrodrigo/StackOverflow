#include <stdio.h>
struct Person {    
    char name[100];    
    short age;
    };
/*
    void display_person(struct Person person) {    
        printf("This person is called %s and is %hd years old\n", person.name, person.age); 
    }

    void display_person(struct Person *person) {   
        printf("This person is called %s and is %hd years old\n", (*person).name, (*person).age);     
    }
*/

   void display_person(struct Person *person) {   
       printf("This person is called %s and is %hd years old\n", person->name, person->age);  
   }
    int main() {
        struct Person example = {
            .name = "Rodrigo",
            .age = 35    
        };    
        
        display_person(&example);     
        
        return 0;
    }