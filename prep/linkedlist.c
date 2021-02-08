#include <stdio.h>
#include <stdlib.h>

typedef struct LinkedListNode LinkedListNode;
struct LinkedListNode {    
    int value;    
    LinkedListNode *next;
}; 

LinkedListNode *new_node(LinkedListNode *previous, int value)
{
    LinkedListNode *output = (LinkedListNode *)malloc(sizeof(LinkedListNode));
    output->value = value;
    if (previous)
        previous->next = output;
    output->next = 0;
    return output;
}
void delete_list(LinkedListNode *node)
{
    while (node)
    {
        LinkedListNode *temp = node->next;
        free(node);
        node = temp;
    }
}
long sum_linked_list(LinkedListNode *start)
{
    long output = 0;
    while (start)
    {
        output += start->value;
        start = start->next;
    }
    return output;
}

int main()
{
    LinkedListNode *list = 0, *temp;
    list = new_node(list, 1);
    temp = new_node(list, 2);
    temp = new_node(temp, 3);
    temp = new_node(temp, 4);
    temp = new_node(temp, 5);
    printf("The sum of the nodes is %ld\n", sum_linked_list(list));
    delete_list(list);
    return 0;
}