# StackOverflow

This repo was created to demonstrate the operation of a buffer overflow exploit, we are going to change a password variable within a vulnerable program which might illustrate for example how can an attacker grant access to an assumed program which requires authentication by overwriting the current/real password in the stack memory of a C program. 

## Understanding the underling concepts 

Understanding stack-based overflow attacks involves at least a basic understanding of computer memory, let's review some very important concepts. 

### Stack memory: 

A stack is the memory which stores variables created by a function, it's temporary, when the program is finished, the memory of the variable will be automatically erased and the area freed up, the stack is managed by the CPU – so you don’t have to allocate or deallocate memory. The stack section mostly contains methods, local variable, and reference variables. 


### Heap memory

The heap is the memory used by programming languages to store large block of memory, it's also known as dynamic memory. Unlike the stack this area of memory is managed by the programmer in C for example, variables are allocated and freed using functions like malloc() and free()

## What is stack overflow exploit?

When the execution stack grows beyond the memory that is reserved for it, you will cause a stack overflow as the stack will eventually consume more memory than is reserved for it and eventually overwrite other buffers in the same stack. 

